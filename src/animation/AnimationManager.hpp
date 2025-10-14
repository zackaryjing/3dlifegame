#pragma once
#include <deque>
#include <queue>
#include <set>
#include <unordered_set>

#include "animation/Animation.hpp"
#include "group/Group.hpp"

using std::deque;
using std::priority_queue;
using std::set;
using std::unordered_set;
using std::vector;

class AnimationManager {
public:
    AnimationManager() = default;
    priority_queue<Animation *, vector<Animation *>, AnimationCompare>
            groupAnimationList;
    set<Animation *> currentAnimation;
    bool inAnimation = false;
    void addAnimation(Animation *animation) {
        groupAnimationList.push(animation);
    };
    void update(const float curTime) {
        while (not groupAnimationList.empty() and
               groupAnimationList.top()->startTime <= curTime) {
            groupAnimationList.top()->begin();
            currentAnimation.insert(groupAnimationList.top());
            groupAnimationList.pop();
        }

        vector<Animation *> toBeRemove;
        for (auto &animation: currentAnimation) {
            animation->update(curTime);
            if (animation->endTime < curTime) {
                if (animation->loop == true) {
                    animation->startTime = curTime;
                    animation->endTime = curTime + animation->duration;
                } else {
                    animation->finalState();
                    toBeRemove.push_back(animation);
                }
            }
        }
        for (auto animation: toBeRemove) {
            currentAnimation.erase(animation);
        }
    }
};
