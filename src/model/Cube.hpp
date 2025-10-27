#pragma once


#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shaders/Shader.hpp"
#include "texture/TextureLoader.hpp"
#include "ui/KeyboardInput.hpp"

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
            -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, -1.0, 0.0,  0.0, // 3
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
            -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0,  -1.0, 0.0, // 0
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0,  -1.0, 0.0, // 4

            -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.0,  1.0,  0.0, // 7
            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0,  1.0,  0.0, // 3
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

        unsigned int texture = createBrickWallTexture();

        glActiveTexture(GL_TEXTURE0); // activate texture unit first
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        ourShader.setInt("ourTexture", 0);


        const glm::vec3 lightDir = normalize(glm::vec3{1, 1, 2});
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while (not glfwWindowShouldClose(window)) {
            processInput(window, deltaTime);
            const glm::mat4 model = glm::rotate(
                    glm::mat4(1.0f),
                    static_cast<float>(glfwGetTime()) * glm::radians(50.0f),
                    glm::vec3(0.5f, 1.0f, 0.0f));

            const glm::mat4 view = Camera::getView();
            const glm::mat4 projection = Camera::getProjection();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ourShader.setVec3("lightDir", lightDir);
            ourShader.setMatrix4("model", model);
            ourShader.setMatrix4("view", view);
            ourShader.setMatrix4("projection", projection);

            glDrawArrays(GL_TRIANGLES, 0, indices_cnt);

            glfwSwapBuffers(window);
            glfwPollEvents();
            glfwSwapInterval(1);

            const float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }
    }
};
