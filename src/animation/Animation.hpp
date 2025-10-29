#pragma once

#include <bitset>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

using std::bitset;
using std::cout;
using std::endl;

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
        modelMat(modelMat), startMat(modelMat) {}
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

class RotatePosAnimation final : public Animation {
public:
    glm::vec3 axis;
    float degree;
    glm::mat4 &modelMat;
    glm::vec3 &position;
    glm::vec3 startPos;
    glm::mat4 startMat;
    RotatePosAnimation(const float duration, const float startTime,
                       const bool loop, const glm::vec3 axis,
                       const float degree, glm::mat4 &modelMat,
                       glm::vec3 &pos) :
        Animation(duration, startTime, loop), axis(axis), degree(degree),
        modelMat(modelMat), position(pos) {
        startPos = position; // dummy
        startMat = modelMat; // dummy
    }
    void begin() override {
        // save start status at the moment when animation starts
        startMat = modelMat;
        startPos = position;
    }


    void update(const float curTime) override {
        modelMat = glm::rotate(glm::mat4(1.0f),
                               glm::radians((curTime - startTime) / duration *
                                            degree),
                               axis) *
                   startMat;
        position = glm::vec3(modelMat[3]);
    }
    void finalState() override {
        modelMat = glm::rotate(glm::mat4(1.0f), glm::radians(degree), axis) *
                   startMat;
        position = glm::vec3(modelMat[3]);
    }
};


class Vec3SinAnimation final : public Animation {
public:
    glm::vec3 amplitude;


    glm::vec3 period;
    glm::vec3 &phase;
    glm::vec3 &vec;
    glm::vec3 startPhase;
    glm::vec3 angularFrequency;
    glm::vec3 offset;
    Vec3SinAnimation(const float duration, const float startTime,
                     const bool loop, glm::vec3 amplitude, glm::vec3 period,
                     glm::vec3 offset, glm::vec3 &phase, glm::vec3 &vec) :
        Animation(duration, startTime, loop), amplitude(amplitude),
        period(period), phase(phase), vec(vec), offset(offset) {
        angularFrequency = glm::vec3(2 * M_PI, 2 * M_PI, 2 * M_PI) / period;
        startPhase = phase;
    }
    void begin() override { startPhase = phase; }


    void update(const float curTime) override {
        float timeDiff = curTime - startTime;
        phase = startPhase + glm::vec3(angularFrequency.x * timeDiff,
                                       angularFrequency.y * timeDiff,
                                       angularFrequency.z * timeDiff);
        vec.x = amplitude.x * sin(phase.x) + offset.x;
        vec.y = amplitude.y * sin(phase.y) + offset.y;
        vec.z = amplitude.z * sin(phase.z) + offset.z;
    }
    void finalState() override {
        phase = startPhase + glm::vec3(angularFrequency.x * duration,
                                       angularFrequency.y * duration,
                                       angularFrequency.z * duration);
        vec.x = amplitude.x * sin(phase.x) + offset.x;
        vec.y = amplitude.y * sin(phase.y) + offset.y;
        vec.z = amplitude.z * sin(phase.z) + offset.z;
    }
};
