#pragma once

#include <bitset>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

using std::bitset;

class Animation {
public:
    float duration;
    float startTime;
    float endTime;
    bool loop;

    Animation(const float duration, const float startTime, const bool loop) :
        duration(duration), startTime(startTime), endTime(startTime + duration),
        loop(loop) {}
    virtual void begin() = 0;
    virtual void update(float curTime) = 0;
    virtual void finalState() = 0;
    virtual ~Animation() = default;
};

struct AnimationCompare {
    bool operator()(const Animation *a, const Animation *b) const {
        return a->startTime > b->startTime;
    }
};

class RotateAnimation final : public Animation {
public:
    glm::vec3 axis;
    float degree;
    glm::mat4 &modelMat;
    glm::mat4 startMat;
    RotateAnimation(const float duration, const float startTime,
                    const bool loop, const glm::vec3 axis, const float degree,
                    glm::mat4 &modelMat) :
        Animation(duration, startTime, loop), axis(axis), degree(degree),
        modelMat(modelMat) {
        startMat = modelMat;
    }
    void begin() override { startMat = modelMat; }


    void update(const float curTime) override {
        modelMat = glm::rotate(glm::mat4(1.0f),
                               glm::radians((curTime - startTime) / duration *
                                            degree),
                               axis) *
                   startMat;
    }
    void finalState() override {
        modelMat = glm::rotate(glm::mat4(1.0f), glm::radians(degree), axis) *
                   startMat;
    }
};
