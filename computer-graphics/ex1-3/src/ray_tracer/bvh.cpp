#include "./ray_tracer.h"
#include <queue>

namespace RT {

BVH::BVH(const std::vector<Triangle>& triangles) : triangles(triangles) {
    nodes.reserve(triangles.size() * 2);
    build(0, triangles.size(), 0);
}

float intersectTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const Triangle& tri) {
    // Möller–Trumbore intersection algorithm
    glm::vec3 edge1 = tri.v1 - tri.v0;
    glm::vec3 edge2 = tri.v2 - tri.v0;
    glm::vec3 h = glm::cross(rayDir, edge2);
    float a = glm::dot(edge1, h);

    if (fabs(a) < 1e-6) return false;

    float f = 1.0 / a;
    glm::vec3 s = rayOrigin - tri.v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0 || u > 1.0) return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayDir, q);
    if (v < 0.0 || u + v > 1.0) return false;

    float t = f * glm::dot(edge2, q);
    return t;
}

Hit BVH::anyHit(const glm::vec3& rayOrigin, const glm::vec3& rayDir) {
    glm::vec3 invDir = 1.0f / rayDir;
    std::vector<int> stack;
    stack.push_back(nodes.size() - 1); // Start with the root node

    while (!stack.empty()) {
        int nodeIndex = stack.back();
        stack.pop_back();
        const BVHNode& node = nodes[nodeIndex];

        if (!node.bounds.intersects(rayOrigin, rayDir, invDir)) {
            continue;
        }

        if (node.isLeaf()) {
            for (int i = node.start; i < node.end; ++i) {
                const Triangle& tri = triangles[i];
                if (float t = intersectTriangle(rayOrigin, rayDir, tri); t > 1e-6) {
                    return Hit(t, glm::normalize(glm::cross(tri.v1 - tri.v0, tri.v2 - tri.v0)), tri.material);
                }
            }
        } else {
            stack.push_back(node.leftChild);
            stack.push_back(node.rightChild);
        }
    }
    return Hit(-1, glm::vec3(0), Material());
}

int BVH::build(int start, int end, int depth) {
    BVHNode node;
    node.start = start;
    node.end = end;

    // Compute the bounding box for the node
    for (int i = start; i < end; ++i) {
        AABB triBounds(triangles[i].getMin(), triangles[i].getMax());
        node.bounds.expand(triBounds);
    }

    // If leaf node
    if (end - start <= 2) {
        int index = nodes.size();
        nodes.push_back(node);
        return index;
    }

    // Find the axis to split along
    glm::vec3 centroidBoundsMin(FLT_MAX), centroidBoundsMax(-FLT_MAX);
    for (int i = start; i < end; ++i) {
        glm::vec3 centroid = triangles[i].getCentroid();
        centroidBoundsMin = glm::min(centroidBoundsMin, centroid);
        centroidBoundsMax = glm::max(centroidBoundsMax, centroid);
    }

    int splitAxis = 0;
    glm::vec3 diff = centroidBoundsMax - centroidBoundsMin;
    if (diff.y > diff.x) splitAxis = 1;
    if (diff.z > diff.y && diff.z > diff.x) splitAxis = 2;

    std::sort(triangles.begin() + start, triangles.begin() + end, [splitAxis](const Triangle& a, const Triangle& b) {
        return a.getCentroid()[splitAxis] < b.getCentroid()[splitAxis];
    });

    int mid = (start + end) / 2;
    node.leftChild = build(start, mid, depth + 1);
    node.rightChild = build(mid, end, depth + 1);

    int index = nodes.size();
    nodes.push_back(node);
    return index;
}

}