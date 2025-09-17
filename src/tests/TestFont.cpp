#include <iostream>
#include "fonts/Font.hpp"
#include "glad/glad.h"

int main() {
    if (!glfwInit()) {
        cerr << "Failed to init glfw" << endl;
        return 0;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cerr << "Failed to init GLAD" << endl;
    }
    FontPixelStyle font;
    font.debugTest();
}
