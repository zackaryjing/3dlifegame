#pragma once

#include <GLFW/glfw3.h>
#include "camera/Camera.hpp"
struct Cursor {
    static inline float lastX = 400.0f, lastY = 300.0f;
    static inline float xoffset = 0.0f, yoffset = 0.0f;
    static constexpr float sensitivity = 0.1f;
    static inline bool firstMouse = true;
    static inline float zoom = 45.0f;
    static inline float moveThreshold = 100.0f;
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
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

        Camera::lookAround(yoffset, xoffset);
    }

    static void scroll_callback(GLFWwindow *window, const double xoffset_param,
                                const double yoffset_param) {
        (void) window;
        (void) xoffset_param;
        (void) yoffset_param;
        zoom -= static_cast<float>(yoffset_param);
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
        Camera::setFov(zoom);
    }
    static void window_focus_callback(GLFWwindow *window, int focused) {
        if (focused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
};
