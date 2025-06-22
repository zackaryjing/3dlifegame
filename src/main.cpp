#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "shaders/shader.h"
#include "init.h"

using namespace std;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(1600, 1200, "LifeGame", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (not gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f

    };

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3,
    };

    // create and bind vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // create vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);

    // vertex attributes pointers
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
    // vertex attribute location
    glEnableVertexAttribArray(0);

    // vertex attributes pointers
    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    // vertex attribute location
    glEnableVertexAttribArray(1);

    unsigned int shaderProgram = genShaderProgram();
    glUseProgram(shaderProgram);


    // bind vertex array object
    glBindVertexArray(VAO);

    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    while (not glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // draw triangles
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);
    }


    return 0;
}
