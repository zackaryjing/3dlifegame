#pragma once
#include <vector>

#include <GLFW/glfw3.h>
#include "group/Group.hpp"

using std::vector;


class Scene {
public:
    string name = "";
    virtual ~Scene() = default;
    virtual void takeControl() = 0;
    Scene() = default;
    Scene(GLFWwindow *window);
    Scene(GLFWwindow *window, const string &name);
    virtual void render() = 0;
    virtual void stop() = 0;
};
