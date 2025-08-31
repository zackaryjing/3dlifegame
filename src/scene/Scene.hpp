#pragma once
#include <algorithm>
#include <limits>
#include <numeric>
#include <ranges>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "light/Light.hpp"
#include "model/Model.hpp"
#include "shaders/Shader.hpp"
#include "texture/TextureLoader.hpp"
#include "ui/KeyboardInput.hpp"

using std::vector;


class Scene {
public:
    static inline string vertex_shader = GLSL_DIR "CubeShader.vs";

    static inline string fragment_shader = GLSL_DIR "CubeShader.fs";

    static inline string lighting_fshader = GLSL_DIR "LightingShader.fs";
    static inline string lighting_vshader = GLSL_DIR "LightingShader.vs";

    static inline vector<shared_ptr<Model>> models = {};

    static vector<float> genVertices() {
        const int total_cnts = ranges::fold_left(
                models, 0, [](const int total, const shared_ptr<Model> &model) {
                    return total + model->vertex_cnt;
                });

        vector<float> vertices;
        vertices.reserve(total_cnts * 8);
        for (const auto &model: models) {
            for (size_t i = 0; i < model->vertex_cnt; ++i) {
                for (int j = 0; j < 3; ++j) {
                    vertices.push_back(model->vertices[i * 3 + j]);
                }
                for (int j = 0; j < 2; ++j) {
                    vertices.push_back(model->texture_coord[i * 2 + j]);
                }
                for (int j = 0; j < 3; ++j) {
                    vertices.push_back(model->normals[i * 3 + j]);
                }
            }
        }
        return vertices;
    }

    /**
     *
     * @return start_pos, cnt
     */
    static vector<pair<int, int>> getIntervals() {
        vector<pair<int, int>> interval;
        interval.reserve(models.size());
        int start_point = 0;
        for (const auto &model: models) {
            interval.emplace_back(start_point, model->vertex_cnt);
            start_point += model->vertex_cnt;
        }
        return interval;
    }

    static vector<glm::mat4> getModelMat() {
        vector<glm::mat4> modelMats;
        modelMats.reserve(models.size());
        for (const auto &model: models) {
            modelMats.push_back(model->modelMat);
        }
        return modelMats;
    }

    static unsigned int getVBO() {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        return VBO;
    }

    static unsigned int getObjectVAO(const unsigned int VBO) {
        constexpr int stride = 8;
        // vertices
        constexpr GLuint vertices_index = 0;
        constexpr GLint vertices_size = 3;
        constexpr GLsizei vertices_stride = stride * sizeof(float);
        const auto vertices_pointer =
                reinterpret_cast<void *>(0 * sizeof(float));

        // texture coordinate
        constexpr GLuint coord_index = 1;
        constexpr GLint coord_size = 2;
        constexpr GLsizei coord_stride = stride * sizeof(float);
        const auto coord_pointer = reinterpret_cast<void *>(3 * sizeof(float));

        // normals
        constexpr GLuint normals_index = 2;
        constexpr GLint normals_size = 3;
        constexpr GLsizei normals_stride = stride * sizeof(float);
        const auto normals_pointer =
                reinterpret_cast<void *>(5 * sizeof(float));

        // create and bind vertex array object
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        // create vertex buffer object
        const auto vertices = genVertices();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
                     vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(vertices_index, vertices_size, GL_FLOAT, GL_FALSE,
                              vertices_stride, vertices_pointer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(coord_index, coord_size, GL_FLOAT, GL_FALSE,
                              coord_stride, coord_pointer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(normals_index, normals_size, GL_FLOAT, GL_FALSE,
                              normals_stride, normals_pointer);
        glEnableVertexAttribArray(2);
        return VAO;
    }

    static void render(GLFWwindow *window) {
        const auto VBO = getVBO();
        const auto objectVAO = getObjectVAO(VBO);
        Light::init();
        const auto lightVAO = Light::getLightVAO(VBO);


        glEnable(GL_DEPTH_TEST);

        const Shader ourShader(vertex_shader, fragment_shader,
                               ShaderParamType::PATH);
        const Shader lightingShader(lighting_vshader, lighting_fshader,
                                    ShaderParamType::PATH);

        unsigned int texture = create_brick_wall_texture();


        glActiveTexture(GL_TEXTURE0); // activate texture unit first
        glBindTexture(GL_TEXTURE_2D, texture);


        const glm::vec3 lightDir = normalize(glm::vec3{1, 1, 2});
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;


        const auto &interval = getIntervals();
        const auto &modelMat = getModelMat();

        while (not glfwWindowShouldClose(window)) {
            processInput(window, deltaTime);

            const glm::mat4 view = Camera::getView();
            const glm::mat4 projection = Camera::getProjection();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto lightPos = glm::vec3(0.0f, 3.0f, 0.0f);
//            auto lightPos =
//                    glm::vec3(glm::rotate(glm::mat4(1.0f),
//                                          static_cast<float>(glfwGetTime()) *
//                                                  glm::radians(50.0f),
//                                          glm::vec3(0.0f, 0.0f, 1.0f)) *
//                              glm::vec4(0.0f, 3.0f, 0.0f, 0.0f));

            ourShader.use();
            ourShader.setInt("ourTexture", 0);
            ourShader.setMatrix4("view", view);
            ourShader.setMatrix4("projection", projection);
            ourShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
            ourShader.setVec3("lightPos", lightPos);
            ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
//            ourShader.setVec3("viewPos",Camera::cameraPos);
            auto model = glm::mat4(1.0f);
            model = glm::translate(model,lightPos);
            model = glm::scale(model,glm::vec3(0.1f,0.1f,0.1f));

            glBindVertexArray(objectVAO);
            ourShader.setMatrix4("model", model);
            glDrawArrays(GL_TRIANGLES, interval[0].first, interval[0].second);

            ourShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            for (size_t i = 1; i < models.size(); ++i) {
                const glm::mat4 model = glm::rotate(
                        models[i]->modelMat,
//                        static_cast<float>(glfwGetTime()) *
                                glm::radians(50.0f),
                        glm::vec3(0.5f, 1.0f, 0.0f));
                ourShader.setMatrix4("model", model);
                glDrawArrays(GL_TRIANGLES, interval[i].first,
                             interval[i].second);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
            glfwSwapInterval(1);

            const float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }
    }
};
