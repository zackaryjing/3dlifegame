#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

class Window {
public:
    static inline int screenHeight;
    static inline int screenWidth;
    static inline int height;
    static inline int width;
    static inline float ratio = 0.8;
    static inline float aspect_ratio = 1.2;
    static inline const char *name = "LifeGame";
    static inline bool hasInit = false;


    static inline void init() {
        if (not hasInit) {
            GLFWmonitor *primary = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(primary);
            screenWidth = mode->width;
            screenHeight = mode->height;
            height = static_cast<int>(ratio * static_cast<float>(screenHeight));
            width = static_cast<int>(ratio * static_cast<float>(screenWidth));
            aspect_ratio = static_cast<float>(screenWidth) /
                           static_cast<float>(screenHeight);
        }
    };
};
