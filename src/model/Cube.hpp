#pragma once

#ifndef GLSL_DIR
#define GLSL_DIR "../src/shaders/" // fallback: 编辑器静态分析用
#endif

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../init.h"
#include "../shaders/shader.h"
#include "../texture/load_texture.h"

struct Cube {
    static constexpr float vertices[] = {
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0,  0.0,  1.0, // 0
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0,  0.0,  1.0, // 1
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0,  0.0,  1.0, // 2

            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0,  0.0,  1.0, // 2
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0,  0.0,  1.0, // 3
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0,  0.0,  1.0, // 0

            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0,  0.0,  -1.0, // 6
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0,  0.0,  -1.0, // 5
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0,  0.0,  -1.0, // 4

            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0,  0.0,  -1.0, // 4
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0,  0.0,  -1.0, // 7
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0,  0.0,  -1.0, // 6

            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -1.0, 0.0,  0.0, // 7
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0, 0.0,  0.0, // 4
            -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, -1.0, 0.0,  0.0, // 0

            -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, -1.0, 0.0,  0.0, // 0
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -1.0, 0.0,  0.0, // 3
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -1.0, 0.0,  0.0, // 7

            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 1.0,  0.0,  0.0, // 1
            0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, 1.0,  0.0,  0.0, // 5
            0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 1.0,  0.0,  0.0, // 6

            0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 1.0,  0.0,  0.0, // 6
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0,  0.0,  0.0, // 2
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 1.0,  0.0,  0.0, // 1

            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0,  -1.0, 0.0, // 4
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0,  -1.0, 0.0, // 5
            0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0,  -1.0, 0.0, // 1

            0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0,  -1.0, 0.0, // 1
            -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0,  -1.0, 0.0, // 0
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0,  -1.0, 0.0, // 4

            -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.0,  1.0,  0.0, // 7
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0,  1.0,  0.0, // 3
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0,  1.0,  0.0, // 2

            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0,  1.0,  0.0, // 2
            0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0,  1.0,  0.0, // 6
            -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.0,  1.0,  0.0, // 7
    };
    static constexpr float inv_sqrt3 = 0.577333;

    static inline string vertex_shader = GLSL_DIR "CubeShader.vs";

    static inline string fragment_shader = GLSL_DIR "CubeShader.fs";

    static constexpr int stride = 8;
    // vertices
    static inline GLuint vertices_index = 0;
    static inline GLint vertices_size = 3;
    static inline GLsizei vertices_stride = stride * sizeof(float);
    static inline auto vertices_pointer =
            reinterpret_cast<void *>(0 * sizeof(float));

    // texture coordinate
    static inline GLuint coord_index = 1;
    static inline GLint coord_size = 2;
    static inline GLsizei coord_stride = stride * sizeof(float);
    static inline auto coord_pointer =
            reinterpret_cast<void *>(3 * sizeof(float));

    // normals
    static inline GLuint normals_index = 2;
    static inline GLint normals_size = 3;
    static inline GLsizei normals_stride = stride * sizeof(float);
    static inline auto normals_pointer =
            reinterpret_cast<void *>(5 * sizeof(float));

    static inline GLsizei vertices_cnt = 8;
    static inline GLsizei indices_cnt = 36;


    static void render(GLFWwindow *window) {

        // create and bind vertex array object
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);


        // create vertex buffer object
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        // vertex attributes pointers
        glVertexAttribPointer(vertices_index, vertices_size, GL_FLOAT, GL_FALSE,
                              vertices_stride, vertices_pointer);
        // vertex attribute location
        glEnableVertexAttribArray(0);

        // texture coordinate attributes pointers
        glVertexAttribPointer(coord_index, coord_size, GL_FLOAT, GL_FALSE,
                              coord_stride, coord_pointer);
        // texture coordinate attribute location
        glEnableVertexAttribArray(1);

        // normal attributes pointers
        glVertexAttribPointer(normals_index, normals_size, GL_FLOAT, GL_FALSE,
                              normals_stride, normals_pointer);
        // normal attribute location
        glEnableVertexAttribArray(2);

        glEnable(GL_DEPTH_TEST);

        const Shader ourShader(vertex_shader, fragment_shader,
                               ShaderParamType::PATH);

        ourShader.use();

        unsigned int texture = create_brick_wall_texture();

        glActiveTexture(GL_TEXTURE0); // activate texture unit first
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        ourShader.setInt("ourTexture", 0);


        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f,
                                      0.1f, 100.0f);


        const glm::vec3 lightDir = normalize(glm::vec3{1, 1, 2});
        while (not glfwWindowShouldClose(window)) {
            auto model = glm::mat4(1.0f);
            model = glm::rotate(model,
                                static_cast<float>(glfwGetTime()) *
                                        glm::radians(50.0f),
                                glm::vec3(0.5f, 1.0f, 0.0f));

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInput(window);

            ourShader.setVec3("lightDir", lightDir);
            ourShader.setMatrix4("model", model);
            ourShader.setMatrix4("view", view);
            ourShader.setMatrix4("projection", projection);

            glDrawArrays(GL_TRIANGLES, 0, indices_cnt);

            glfwSwapBuffers(window);
            glfwPollEvents();
            glfwSwapInterval(1);
        }
    }
};
