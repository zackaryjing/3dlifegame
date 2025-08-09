#pragma once

#include "../shaders/shader.hpp"
#include "../texture/textureLoader.hpp"

struct CubeEBO {
    static constexpr float vertices[] = {
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
    };
    static constexpr int indices[] = {
            0, 1, 2, //
            2, 3, 0, //
            6, 5, 4, //
            4, 7, 6, //

            7, 4, 0, //
            0, 3, 7, //
            1, 5, 6, //
            6, 2, 1, //

            4, 5, 1, //
            1, 0, 4, //
            7, 3, 2, //
            2, 6, 7, //
    };
    static constexpr float inv_sqrt3 = 0.577333;

    static constexpr float normals[] = {
            -inv_sqrt3, -inv_sqrt3, inv_sqrt3, //
            inv_sqrt3,  -inv_sqrt3, inv_sqrt3, //
            inv_sqrt3,  inv_sqrt3,  inv_sqrt3, //
            -inv_sqrt3, inv_sqrt3,  inv_sqrt3, //
            -inv_sqrt3, -inv_sqrt3, -inv_sqrt3, //
            inv_sqrt3,  -inv_sqrt3, -inv_sqrt3, //
            inv_sqrt3,  inv_sqrt3,  -inv_sqrt3, //
            -inv_sqrt3, inv_sqrt3,  -inv_sqrt3, //
    };

    static inline auto vertex_shader =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec2 aTexCoord;\n"
            "layout (location = 2) in vec3 aNormal;\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "out vec3 normal;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "uniform vec3 lightDir;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
            "    normal = mat3(transpose(inverse(model))) * aNormal;\n"
            "    ourColor = vec3(0.0,0.0,1.0);\n"
            "    TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
            "}";

    static inline auto fragment_shader =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"
            "in vec3 normal;\n"
            "\n"
            "uniform sampler2D ourTexture;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(normal * 0.5 + vec3(0.5), 1.0);\n"
            "}";


    // vertices
    static inline GLuint vertices_index = 0;
    static inline GLint vertices_size = 3;
    static inline GLsizei vertices_stride = 5 * sizeof(float);
    static inline auto vertices_pointer =
            reinterpret_cast<void *>(0 * sizeof(float));

    // texture coordinate
    static inline GLuint coord_index = 1;
    static inline GLint coord_size = 2;
    static inline GLsizei coord_stride = 5 * sizeof(float);
    static inline auto coord_pointer =
            reinterpret_cast<void *>(3 * sizeof(float));

    // normals
    static inline GLuint normals_index = 2;
    static inline GLint normals_size = 3;
    static inline GLsizei normals_stride = 3 * sizeof(float);
    static inline auto normals_pointer =
            reinterpret_cast<void *>(0 * sizeof(float));

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

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
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

        unsigned int normalVBO;
        glGenBuffers(1, &normalVBO);

        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
        // normal attributes pointers
        glVertexAttribPointer(normals_index, normals_size, GL_FLOAT, GL_FALSE,
                              normals_stride, normals_pointer);
        // normal attribute location
        glEnableVertexAttribArray(2);

        glEnable(GL_DEPTH_TEST);

        const Shader ourShader(vertex_shader, fragment_shader,
                               ShaderParamType::CODE);

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

            glDrawElements(GL_TRIANGLES, indices_cnt, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
            glfwSwapInterval(1);
        }
    }
};

// -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
// 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
// -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
// -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
// 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
// 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
// -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //


// 0.0f,  0.0f,  1.0f, //
// 0.0f,  0.0f,  1.0f, //
// 0.0f,  0.0f,  -1.0f, //
// 0.0f,  0.0f,  -1.0f, //
// -1.0f, 0.0f,  0.0f, //
// -1.0f, 0.0f,  0.0f, //
// 1.0f,  0.0f,  0.0f, //
// 1.0f,  0.0f,  0.0f, //
// 0.0f,  -1.0f, 0.0f, //
// 0.0f,  -1.0f, 0.0f, //
// 0.0f,  1.0f,  0.0f, //
// 0.0f,  1.0f,  0.0f, //
