#pragma once
#include <algorithm>
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
            animationList;
    set<Animation *> currentAnimation;
    set<Animation *> toBeRemove;
    bool inAnimation = false;
    void addAnimation(Animation *animation) {
        // due to lazy deletion, animation previously set to be deleted
        // won't be able to added back since every time it was added, it will
        // be deleted together immediately
        toBeRemove.erase(animation);
        animationList.push(animation);
    };
    void update(const float curTime) {
        while (not animationList.empty() and
               animationList.top()->startTime <= curTime) {
            if (not toBeRemove.empty() and
                toBeRemove.contains(animationList.top())) {
                auto temp = animationList.top();
                while (not animationList.empty() and
                       animationList.top() == temp) {
                    cout << "toBeRemoved item in animationList" << endl;
                    animationList.pop();
                }
                toBeRemove.erase(temp);
                cout << "toBeRemoved removed item" << endl;
            } else {
                animationList.top()->begin();
                currentAnimation.insert(animationList.top());
                animationList.pop();
            }
        }

        vector<Animation *> endedAnimation;
        for (auto &animation: currentAnimation) {
            animation->update(curTime);
            if (animation->endTime < curTime) {
                if (animation->loop == true) {
                    animation->startTime = curTime;
                    animation->endTime = curTime + animation->duration;
                } else {
                    animation->finalState();
                    endedAnimation.push_back(animation);
                }
            }
        }

        for (auto animation: endedAnimation) {
            currentAnimation.erase(animation);
        }
    }
    void removeAnimation(Animation *animation) {
        currentAnimation.erase(animation);
        toBeRemove.insert(animation);
    }
};
