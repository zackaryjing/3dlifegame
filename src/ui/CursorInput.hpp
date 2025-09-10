#pragma once

#include <GLFW/glfw3.h>
#include "camera/Camera.hpp"
class Cursor {
public:
    float lastX = 400.0f, lastY = 300.0f;
    float xoffset = 0.0f, yoffset = 0.0f;
    float sensitivity = 0.1f;
    bool firstMouse = true;
    float zoom = 45.0f;
    float moveThreshold = 100.0f;
    Camera &camera;
    Cursor(Camera &camera) : camera(camera) {}
    void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
        (void) window;
        if (firstMouse) {
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = false;
        }
        xoffset = static_cast<float>(xpos) - lastX;
        yoffset = lastY - static_cast<float>(ypos);
        if (fabs(xoffset) > moveThreshold || fabs(yoffset) > moveThreshold) {
            yoffset = 0;
            xoffset = 0;
        }
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.lookAround(yoffset, xoffset);
    }

    void scroll_callback(GLFWwindow *window, const double xoffset_param,
                         const double yoffset_param) {
        (void) window;
        (void) xoffset_param;
        (void) yoffset_param;
        zoom -= static_cast<float>(yoffset_param);
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
        camera.setFov(zoom);
    }
    void window_focus_callback(GLFWwindow *window, int focused) {
        if (focused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
};


class GlobalCursor {
public:
    inline static Cursor *cursor = nullptr;
    static void setCursor(Cursor *newCursor) { cursor = newCursor; }
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
        cursor->mouse_callback(window, xpos, ypos);
    }
    static void scroll_callback(GLFWwindow *window, const double xoffset_param,
                                const double yoffset_param) {
        cursor->scroll_callback(window, xoffset_param, yoffset_param);
    }
    static void window_focus_callback(GLFWwindow *window, int focused) {
        cursor->window_focus_callback(window, focused);
    }
};
