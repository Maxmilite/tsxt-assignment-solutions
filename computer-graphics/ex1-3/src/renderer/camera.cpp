#include "./renderer.h"

Camera::Camera() {

    auto& globalConfig = getGlobalConfig();

    position = globalConfig.getVec3("camera.position", glm::vec3(0.0f, 0.0f, 3.0f));
    worldUp = globalConfig.getVec3("camera.worldUp", glm::vec3(0.0f, 1.0f, 0.0f));
    front = globalConfig.getVec3("camera.front", glm::vec3(0.0f, 0.0f, -1.0f));
    yaw = globalConfig.get("camera.yaw", -90.0f);
    pitch = globalConfig.get("camera.pitch", 0.0f);
    speed = globalConfig.get("camera.speed", 2.5f);
    sensitivity = globalConfig.get("camera.sensitivity", 0.1f);
    fov = globalConfig.get("camera.fov", 75.0f);
    near_distance = globalConfig.get("camera.near", 0.1f);
    far_distance = globalConfig.get("camera.far", 100.0f);
    update();
}

Camera::Camera(
    glm::vec3 position, 
    glm::vec3 up,
    float yaw, 
    float pitch
) {

    auto& globalConfig = getGlobalConfig();

    this->position = position;
    worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    front = globalConfig.getVec3("camera.front", glm::vec3(0.0f, 0.0f, -1.0f));
    speed = globalConfig.get("camera.speed", 2.5f);
    sensitivity = globalConfig.get("camera.sensitivity", 0.1f);
    fov = globalConfig.get("camera.fov", 75.0f);
    near_distance = globalConfig.get("camera.near", 0.1f);
    far_distance = globalConfig.get("camera.far", 100.0f);
    update();
}

void Camera::moveForward(float deltaTime) {
    position += speed * front * deltaTime;
    update();
}

void Camera::moveBackward(float deltaTime) {
    position -= speed * front * deltaTime;
    update();
}

void Camera::moveLeft(float deltaTime) {
    position -= speed * right * deltaTime;
    update();
}

void Camera::moveRight(float deltaTime) {
    position += speed * right * deltaTime;
    update();
}

void Camera::moveUp(float deltaTime) {
    position += speed * up * deltaTime;
    update();
}

void Camera::moveDown(float deltaTime) {
    position -= speed * up * deltaTime;
    update();
}

void Camera::rotateLeft(float offset) {
    yaw -= offset * sensitivity;
    update();
}

void Camera::rotateRight(float offset) {
    yaw += offset * sensitivity;
    update();
}

void Camera::rotateUp(float offset) {
    pitch += offset * sensitivity;

    update();
}

void Camera::rotateDown(float offset) {
    pitch -= offset * sensitivity;

    update();
}

void Camera::update() {

    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    {
        int width, height;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

        aspect = (float) width / (float) height;
    }
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspect, near_distance, far_distance);
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getFront() {
    return front;
}

glm::vec3 Camera::getUp() {
    return up;
}

glm::vec3 Camera::getRight() {
    return right;
}

float Camera::getYaw() {
    return yaw;
}

float Camera::getPitch() {
    return pitch;
}

float Camera::getSpeed() {
    return speed;
}

float Camera::getSensitivity() {
    return sensitivity;
}

float Camera::getFov() {
    return fov;
}

float Camera::getAspect() {
    return aspect;
}

float Camera::getNear() {
    return near_distance;
}

float Camera::getFar() {
    return far_distance;
}

void Camera::setSpeed(float speed) {
    this->speed = speed;
}

void Camera::setSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
}

void Camera::setFov(float fov) {
    this->fov = fov;
}

void Camera::setAspect(float aspect) {
    this->aspect = aspect;
}

void Camera::setNear(float near_distance) {
    this->near_distance = near_distance;
}

void Camera::setFar(float far_distance) {
    this->far_distance = far_distance;
}