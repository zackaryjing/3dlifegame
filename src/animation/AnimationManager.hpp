#pragma once
#include <deque>
#include "animation/Animation.hpp"

using std::deque;

class AnimationManager {
    deque<Animation> animationList;
    void addAnimation();
};