#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class Animation {
public:
    float duration;
    float startTime;
    bool loop;

    Animation(const float duration, const float startTime, const bool loop) :
        duration(duration), startTime(startTime), loop(loop) {}
    virtual void update(float curTime) = 0;
    virtual ~Animation();
};


class RotateAnimation : public Animation {
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
    void update(const float curTime) override {
        modelMat = glm::rotate(
                startMat,
                glm::radians((curTime - startTime) / duration * degree), axis);
    }
};
