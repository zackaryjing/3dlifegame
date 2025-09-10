#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "camera/Camera.hpp"

using namespace std;

class Keyboard {
    virtual void processInput();
}

class KeyboardMoveControl : Keyboard {
public:
    void processInput(GLFWwindow *window, float deltaTime, Camera &camera) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveBackward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.moveUpward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.moveDownward(deltaTime);
        }
    }

};

class KeyboardManager() {
public:
    static Keyboard keyboard;
    static setKeyboard(Keyboard& newKeyboard) {
        keyboard = newKeyboard;
    }
    static void processInput(GLFWwindow *window, float deltaTime, Camera &camera) {
        keyboard.processInput(GLFWwindow *window, float deltaTime, Camera &camera)
    }
}