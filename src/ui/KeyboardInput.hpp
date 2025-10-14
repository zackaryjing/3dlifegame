#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "camera/Camera.hpp"

using namespace std;

class Keyboard {
public:
    inline virtual void processInput(GLFWwindow *window, float deltaTime,
                                     Camera &camera) {
        (void) window;
        (void) deltaTime;
        (void) camera;
    };
};

class KeyboardUiControl : public Keyboard {
public:
    void processInput(GLFWwindow *window, float deltaTime,
                      Camera &camera) override {
        (void) deltaTime;
        (void) camera;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            Window::isAccessingUI = false;
            Window::updateCursorState(window);
        }
        if (not Window::isAccessingUI) {
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            Window::isAccessingUI = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Window::updateCursorState(window);
        }
    }
};


class KeyboardMoveControl : public Keyboard {
public:
    void processInput(GLFWwindow *window, float deltaTime,
                      Camera &camera) override {

        if (not Window::isAccessingUI) {
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
    }
};

class GlobalKeyboard {
public:
    inline static Keyboard *keyboard = nullptr;
    inline static Keyboard *keyboard_ui = new KeyboardUiControl;
    static void setKeyboard(Keyboard *newKeyboard) { keyboard = newKeyboard; }
    static void processInput(GLFWwindow *window, float deltaTime,
                             Camera &camera) {
        keyboard_ui->processInput(window, deltaTime, camera);
        keyboard->processInput(window, deltaTime, camera);
    }
};
