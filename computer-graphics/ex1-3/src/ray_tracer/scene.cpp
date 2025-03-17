#include "./ray_tracer.h"

namespace RT {

Scene::Scene() : lightPos(glm::vec3(0.0f)), lightRadiance(glm::vec3(1.0f)) {

    // Define walls
    addWall(Wall(
        glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), 
        glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), 
        glm::vec3(0.5f, 0.5f, 0.5f)
    )); // Back wall
    addWall(Wall(
        glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), 
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), 
        glm::vec3(0.5f, 0.5f, 0.5f)
    )); // Front wall
    addWall(Wall(
        glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), 
        glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), 
        glm::vec3(0.5f, 0.5f, 0.5f)
    )); // Left wall
    addWall(Wall(
        glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(0.5f, 0.5f, 0.5f)
    )); // Right wall
    addWall(Wall(
        glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), 
        glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), 
        glm::vec3(0.05f, 0.02f, 0.0f)
    )); // Floor (brown)
    addWall(Wall(
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),  
        glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),  
        glm::vec3(0.5f, 0.5f, 0.5f)
    )); // Ceiling

    // Define spheres
    addSphere(Sphere(glm::vec3(-0.5f, -0.5f, -0.5f), 0.4f, glm::vec3(5.0f, 0.1f, 0.0f))); // Red sphere
    addSphere(Sphere(glm::vec3(0.5f, -0.5f, -0.5f), 0.4f, glm::vec3(0.0f, 0.5f, 5.0f))); // Blue sphere

    // Set light
    setLight(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(4.f, 4.f, 1.5f), 0.2f);

}

void Scene::addWall(const Wall& wall) {
    walls.push_back(wall);
}

void Scene::addSphere(const Sphere& sphere) {
    spheres.push_back(sphere);
}

void Scene::setLight(const glm::vec3& lightPos, const glm::vec3& lightRadiance, float lightRadius) {
    this->lightPos = lightPos;
    this->lightRadiance = lightRadiance;
    this->lightRadius = lightRadius;
}

}