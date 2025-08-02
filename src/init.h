#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

inline void processInput(GLFWwindow *window) {
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) {
        cout << "A Pressed" << endl;
    }
    if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) {
        cout << "D Pressed" << endl;
    }
}

inline void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
