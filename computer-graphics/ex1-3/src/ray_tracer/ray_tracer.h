#ifndef RAY_TRACER_H__
#define RAY_TRACER_H__

namespace RT {
    class RayTracer;
    class BVH;
    class Mesh;
    struct Material;
    struct Triangle;
}

#include "../renderer/renderer.h"
#include "../compute_shader/compute_shader.h"


// deprecated. can be folded

namespace RT {

const int MAX_LEAF_SIZE = 8;

struct Material {
    glm::vec3 color;
};

struct Triangle {
    glm::vec3 v0, v1, v2;
    glm::vec3 n0, n1, n2;

    glm::vec3 getMin() const {
        return glm::min(glm::min(v0, v1), v2);
    }

    glm::vec3 getMax() const {
        return glm::max(glm::max(v0, v1), v2);
    }

    glm::vec3 getCentroid() const {
        return (v0 + v1 + v2) / 3.0f;
    }

    Material material;
};

struct Hit {
    float t;
    glm::vec3 normal;
    Material material;
};

struct AABB {
    glm::vec3 min, max;

    AABB() : min(glm::vec3(FLT_MAX)), max(glm::vec3(-FLT_MAX)) {}
    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    void expand(const AABB& other) {
        min = glm::min(min, other.min);
        max = glm::max(max, other.max);
    }

    bool intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& invDir) const {
        glm::vec3 t0 = (min - rayOrigin) * invDir;
        glm::vec3 t1 = (max - rayOrigin) * invDir;
        glm::vec3 tMin = glm::min(t0, t1);
        glm::vec3 tMax = glm::max(t0, t1);

        float tEnter = glm::max(tMin.x, glm::max(tMin.y, tMin.z));
        float tExit = glm::min(tMax.x, glm::min(tMax.y, tMax.z));
        return tEnter <= tExit && tExit >= 0.0f;
    }
};

struct Ray {
    glm::vec3 orig, dir;
};

class Mesh {
public:
    void loadModel(std::unique_ptr<Model>& model);
    std::vector<Triangle>& getTriangles() { return triangles; }

private:
    std::vector<Triangle> triangles;
};

class BVH {
public:

    struct BVHNode {
        AABB bounds;
        int leftChild = -1, rightChild = -1;
        int start = -1, end = -1; // For leaf nodes, indices to triangles

        bool isLeaf() const {
            return start != -1;
        }
    };
    BVH(const std::vector<Triangle>& triangles);
    BVH() = default;

    Hit anyHit(const glm::vec3& rayOrigin, const glm::vec3& rayDir);

private:
    std::vector<Triangle> triangles;
    std::vector<BVHNode> nodes;
    int root;
    int build(int l, int r, int n);

};

}


namespace RT {

class Scene {

public:
    struct Wall {
        glm::vec3 p00;
        float padding1;
        glm::vec3 p01;
        float padding2;
        glm::vec3 p10;
        float padding3;
        glm::vec3 p11;
        float padding4;
        glm::vec3 color;
        float padding5;
        Wall(
            const glm::vec3& p00, const glm::vec3& p01, 
            const glm::vec3& p10, const glm::vec3& p11, 
            const glm::vec3& color) : p00(p00), p01(p01), p10(p10), p11(p11), color(color) {
                padding1 = padding2 = padding3 = padding4 = padding5 = 0.0f;
            }
    };

    struct Sphere {
        glm::vec3 center;
        float padding1;
        glm::vec3 color;
        float padding2;
        float radius;
        float padding3[3];
        Sphere(const glm::vec3& center, float radius, const glm::vec3& color) : center(center), radius(radius), color(color) {
            padding1 = padding2 = 0.0f;
            padding3[0] = padding3[1] = padding3[2] = 0.0f;
        }
    };

    Scene();
    void addWall(const Wall& wall);
    void addSphere(const Sphere& sphere);
    void setLight(const glm::vec3& lightPos, const glm::vec3& lightRadiance, float lightRadius);
    std::vector<Wall> walls;
    std::vector<Sphere> spheres;
    glm::vec3 lightPos;
    glm::vec3 lightRadiance;
    float lightRadius;

};

class RayTracer {
public:
    RayTracer();
    void init();
    void render();
    void terminate();

    void resize(int width, int height);

    Scene& getScene() { return scene; }

    void rerender() { shouldRerender = true; }

    int getMaxBounces() { return maxBounces; }
    void setMaxBounces(int bounces) { maxBounces = bounces; }
    int getSPP() { return spp; }
    void setSPP(int spp) { this->spp = spp; }
    std::tuple<glm::vec3, glm::vec3, float> getLight() { return {scene.lightPos, scene.lightRadiance, scene.lightRadius}; }
    void setLight(const glm::vec3& lightPos, const glm::vec3& lightRadiance, float lightRadius) { scene.setLight(lightPos, lightRadiance, lightRadius); }
    

private:
    int width, height;

    std::unique_ptr<ComputeShader> traceShader;
    std::unique_ptr<Shader> screenShader;

    Scene scene;

    unsigned int screenQuadVAO, screenQuadVBO;
    unsigned int wallsSSBO, spheresSSBO, outputTexture;

    int maxBounces;
    int spp;

    std::vector<glm::vec4> image;

    bool shouldRerender;

    void setupScreenRendering();
    void setupBuffers();
    void screenRender();

};

}

#endif