#pragma once

#include <GLFW/glfw3.h>
#include "camera/camera.hpp"
struct Cursor {
    static inline float lastX = 400.0f, lastY = 300.0f;
    static inline float xoffset = 0.0f, yoffset = 0.0f;
    static constexpr float sensitivity = 0.1f;
    static inline bool firstMouse = true;
    static inline float zoom = 45.0f;
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
        if (firstMouse) {
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = false;
        }
        xoffset = static_cast<float>(xpos) - lastX;
        yoffset = lastY - static_cast<float>(ypos);
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Camera::lookAround(yoffset, xoffset);
    }

    static void scroll_callback(GLFWwindow *window, const double xoffset,
                                const double yoffset) {
        zoom -= static_cast<float>(yoffset);
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
        Camera::setFov(zoom);
    }
};
