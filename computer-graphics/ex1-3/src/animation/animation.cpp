#include "./animation.h"
#include "../renderer/renderer.h"

Animation::Animation() {
    currentTime = 0.0f;
    currentKeyframe = 0;
}

Animation::~Animation() {
}

void Animation::addKeyframe(Motion keyframe, float time) {
    keyframes.push_back({keyframe, time});
    reset();
    updateMaxTime();
}

void Animation::update(float deltaTime) {
    currentTime += deltaTime;

    if (currentTime > maxTime) {
        currentTime = maxTime;
    }

    while (
        currentKeyframe + 1 < (int) keyframes.size() && 
        currentTime > keyframes[currentKeyframe + 1].second
    ) {
        currentKeyframe++;
    }
}

void Animation::reset() {
    currentTime = 0.0f;
    currentKeyframe = 0;
    getRenderer().playAnimation(false);
}

void Animation::clearKeyframes() {
    keyframes.clear();
    reset();
    updateMaxTime();
}

void Animation::removeKeyframe(int index) {
    keyframes.erase(keyframes.begin() + index);
    reset();
    updateMaxTime();
}

std::vector<std::pair<Motion, float>>& Animation::getKeyframes() {
    return keyframes;
}

Motion Animation::getCurrentMotion() {
    if (keyframes.empty()) {
        return Motion();
    }

    if (currentKeyframe + 1 == (int) keyframes.size()) {
        return keyframes[currentKeyframe].first;
    }

    float t = (currentTime - keyframes[currentKeyframe].second) / 
                (keyframes[currentKeyframe + 1].second - keyframes[currentKeyframe].second + 1e-6);

    return {
        glm::mix(keyframes[currentKeyframe].first.translation, keyframes[currentKeyframe + 1].first.translation, t),
        glm::slerp(keyframes[currentKeyframe].first.rotation, keyframes[currentKeyframe + 1].first.rotation, t),
        glm::mix(keyframes[currentKeyframe].first.scale, keyframes[currentKeyframe + 1].first.scale, t)
    };
}

void Animation::setCurrentTime(float time) {
    currentTime = time;
    if (currentTime < 0.0f) {
        currentTime = 0.0f;
    }
    if (currentTime > maxTime) {
        currentTime = maxTime;
    }

    currentKeyframe = 0;
    while (
        currentKeyframe + 1 < (int) keyframes.size() && 
        currentTime > keyframes[currentKeyframe + 1].second
    ) {
        currentKeyframe++;
    }
}

void Animation::updateMaxTime() {
    maxTime = 0.0f;

    for (auto& keyframe : keyframes) {
        maxTime = std::max(maxTime, keyframe.second);
    }
}