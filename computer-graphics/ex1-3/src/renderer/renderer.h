#ifndef RENDERER_H__
#define RENDERER_H__

class Shader;
class Light;
class Camera;
class Texture;
class Renderer;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "../objects/objects.h"
#include "../utils/yamlLoader.h"
#include "../gui/gui.h"
#include "../animation/animation.h"
#include "../ray_tracer/ray_tracer.h"
#include "../compute_shader/compute_shader.h"

using RT::RayTracer;

namespace Functional {
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    unsigned int compileShader(const std::string& path, GLenum type);
    unsigned int createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    glm::vec3 calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    void calcVerticesNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    float rayTriangleIntersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
};

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader();
    void use();
    unsigned int getID();
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setMat4(const std::string& name, const float* value);
    void setVec3(const std::string& name, const float* value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec2(const std::string& name, const float* value);
    void setVec2(const std::string& name, float x, float y);

private:
    unsigned int ID;
};

class Light {
public:
    Light();
    Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    void setPosition(glm::vec3 position);
    void setAmbient(glm::vec3 ambient);
    void setDiffuse(glm::vec3 diffuse);
    void setSpecular(glm::vec3 specular);
    glm::vec3 getPosition();
    glm::vec3 getColor();
    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();

private:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Camera {
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void rotateLeft(float offset);
    void rotateRight(float offset);
    void rotateUp(float offset);
    void rotateDown(float offset);
    void update();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::vec3 getUp();
    glm::vec3 getRight();
    float getYaw();
    float getPitch();
    float getSpeed();
    float getSensitivity();
    float getFov();
    float getAspect();
    float getNear();
    float getFar();
    void setSpeed(float speed);
    void setSensitivity(float sensitivity);
    void setFov(float fov);
    void setAspect(float aspect);
    void setNear(float near);
    void setFar(float far);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float fov;
    float aspect;
    float near_distance;
    float far_distance;
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();
    void render();
    void terminate();
    bool shouldClose();
    void setModel(Model& model);
    void setModel();
    void loadShader();
    void resizeWindow(int width, int height);
    Camera& getCamera() { return camera; }
    Light& getLight() { return light; }
    GLFWwindow* getWindow() { return window; }
    std::pair<int, int> getWindowSize() { return {width, height}; }
    void processInput(float deltaTime);
    bool getFocus() { return focus; }
    void setFocus(bool focus) { 
        this->focus = focus; 
        if (focus) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    bool shouldShowLighting() { return showLighting; }
    void setLighting(bool showLighting) { this->showLighting = showLighting; }

    Motion& getMotion() { return motion; }
    void setMotion(Motion motion) { this->motion = motion; }
    glm::vec3& getTranslation() { return motion.translation; }
    glm::quat& getRotation() { return motion.rotation; }
    glm::vec3 getRotationByEulerAngles() {
        return glm::eulerAngles(motion.rotation);
    }
    glm::vec3& getScale() { return motion.scale; }
    void setTranslation(glm::vec3 translation) { this->motion.translation = translation; }
    void setRotation(glm::quat rotation) { this->motion.rotation = rotation; }
    void setRotationByEulerAngles(glm::vec3 eulerAngles) {
        this->motion.rotation = glm::quat(eulerAngles);
    }
    void setScale(glm::vec3 scale) { this->motion.scale = scale; }

    std::pair<glm::vec3, glm::vec3> getRay();
    glm::vec3 getMouseModelIntersection();

    std::unique_ptr<Model>& getModel() { return model; }
    std::unique_ptr<RayTracer>& getRayTracer() { return rayTracer; }

    void handleInteractiveTransformation(int type);

    glm::mat4 getModelMatrix() {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        auto motion = getCurrentMotion();
        modelMatrix = glm::translate(modelMatrix, motion.translation);
        modelMatrix = modelMatrix * glm::mat4_cast(motion.rotation);
        modelMatrix = glm::scale(modelMatrix, motion.scale);
        return modelMatrix;
    }

    enum RenderingMode {
        NONE = 0,
        RASTERIZATION = 1,
        RAYTRACING = 2,
        NORMAL = 3,
        DEPTH = 4
    };

    void setRenderingMode(RenderingMode mode) { renderingMode = mode; }
    RenderingMode& getRenderingMode() { return renderingMode; }

    void playAnimation(bool play) { isAnimationPlaying = play; }
    void setAnimation(Animation& animation) { this->animation = animation; }
    void setUseAnimation(bool use) { useAnimation = use; }

    bool isPlayingAnimation() { return isAnimationPlaying; }
    bool isUsingAnimation() { return useAnimation; }

    Animation& getAnimation() { return animation; }


private:
    GLFWwindow* window;
    int width, height;
    int fpsLimit;
    bool focus;
    bool showLighting;

    std::vector<Shader> shaders;
    Camera camera;
    Light light;
    std::unique_ptr<Model> model;
    Motion motion;

    Animation animation;
    bool isAnimationPlaying;
    bool useAnimation;

    RenderingMode renderingMode;

    void handleRasterization();
    void handleRaytracing();
    void handleNormal();
    void handleDepth();
    Motion getCurrentMotion() {
        if (useAnimation) {
            return animation.getCurrentMotion();
        }
        return motion;
    }

    std::unique_ptr<RayTracer> rayTracer;

};

Renderer& getRenderer();

#endif