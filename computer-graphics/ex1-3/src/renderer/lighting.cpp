#include "./renderer.h"

Light::Light() {
    auto& config = getGlobalConfig();
    
    position = config.getVec3("light.position", glm::vec3(0.0f, 0.0f, 0.0f));
    ambient = config.getVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    diffuse = config.getVec3("light.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
    specular = config.getVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
}

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    this->position = position;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

void Light::setPosition(glm::vec3 position) {
    this->position = position;
}

void Light::setAmbient(glm::vec3 ambient) {
    this->ambient = ambient;
}

void Light::setDiffuse(glm::vec3 diffuse) {
    this->diffuse = diffuse;
}

void Light::setSpecular(glm::vec3 specular) {
    this->specular = specular;
}

glm::vec3 Light::getPosition() {
    return position;
}

glm::vec3 Light::getAmbient() {
    return ambient;
}

glm::vec3 Light::getDiffuse() {
    return diffuse;
}

glm::vec3 Light::getSpecular() {
    return specular;
}