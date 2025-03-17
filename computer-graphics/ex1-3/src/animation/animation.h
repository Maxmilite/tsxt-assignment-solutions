#ifndef ANIMATION_H__
#define ANIMATION_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <iostream>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Motion {
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;

    void reset() {
        translation = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }
};

class Animation {
public:
    Animation();
    ~Animation();

    void addKeyframe(Motion keyframe, float time);
    void update(float deltaTime);
    void reset();
    void clearKeyframes();
    void removeKeyframe(int index);
    std::vector<std::pair<Motion, float>>& getKeyframes();
    
    Motion getCurrentMotion();
    float getCurrentTime() { return currentTime; }
    int getCurrentKeyframe() { return currentKeyframe; }
    void setCurrentTime(float time);

    float getMaxTime() { return maxTime; }

private:
    std::vector<std::pair<Motion, float>> keyframes;

    float currentTime;
    int currentKeyframe;

    float maxTime;
    void updateMaxTime();
};

#endif