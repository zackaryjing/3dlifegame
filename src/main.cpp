#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unistd.h>
#include "init.h"
#include "model/shapes.h"
#include "shaders/shader.h"
#include "texture/load_texture.h"
#ifndef GLSL_DIR
#define GLSL_DIR "../src/shaders/" // fallback: 编辑器静态分析用
#endif

using namespace std;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window =
            glfwCreateWindow(800, 600, "LifeGame", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (not gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "Maximum nr of vertex attributes supported: " << nrAttributes
         << endl;

    unsigned int indices[] = {
            0, 1, 3, 1, 2, 3,
    };

    // create and bind vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // create vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::vertices), Cube::vertices,
                 GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Cube::indices), Cube::indices,
                 GL_STATIC_DRAW);


    // vertex attributes pointers
    glVertexAttribPointer(Cube::vertices_index, Cube::vertices_size, GL_FLOAT,
                          GL_FALSE, Cube::vertices_stride,
                          Cube::vertices_pointer);
    // vertex attribute location
    glEnableVertexAttribArray(0);

    // vertex attributes pointers
    glVertexAttribPointer(Cube::coord_index, Cube::coord_size, GL_FLOAT,
                          GL_FALSE, Cube::coord_stride,
                          Cube::coord_pointer);
    // vertex attribute location
    glEnableVertexAttribArray(1);

    // // vertex attributes pointers
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    //                       reinterpret_cast<void *>(6 * sizeof(float)));
    // // vertex attribute location
    // glEnableVertexAttribArray(2);
    glEnable(GL_DEPTH_TEST);


    // const Shader ourShader("./shaders/shader.fs",
    // "./shaders/shader.vs");
    const Shader ourShader(GLSL_DIR "shader.vs", GLSL_DIR "shader.fs");

    ourShader.use();

    unsigned int texture = create_brick_wall_texture();

    glActiveTexture(GL_TEXTURE0); // activate texture unit first
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    ourShader.setInt("ourTexture", 0);


    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
                                  100.0f);


    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    while (not glfwWindowShouldClose(window)) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f),
                            glm::vec3(0.5f, 1.0f, 0.0f));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);

        const auto timeValue = static_cast<float>(glfwGetTime());
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        ourShader.setFloat("greenColor", {0.0f, greenValue, 0.0f});

        ourShader.setMatrix4("model", model);
        ourShader.setMatrix4("view", view);
        ourShader.setMatrix4("projection", projection);
        // draw triangles
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        glDrawElements(GL_TRIANGLES, Cube::indices_cnt, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);
    }

    return 0;
}
