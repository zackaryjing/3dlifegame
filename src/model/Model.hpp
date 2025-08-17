// Add more models in one scene
#pragma once

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <random>
using std::copy_n;
using std::make_unique;
using std::tuple;
using std::unique_ptr;


// model shouldn't have a render function.
// render a model is managed by the secne
class Model {
public:
    Model() = default;
    unique_ptr<float[]> vertices = {};
    unique_ptr<float[]> texture_coord = {};
    unique_ptr<float[]> normals = {};
    int vertex_cnt = {};

    glm::vec3 position = {};
    glm::mat4 modelMat = {};

    static glm::vec3 randomVec3(const float min, const float max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution dis(min, max);
        return glm::vec3(dis(gen), dis(gen), dis(gen));
    }

    static Model getCube() {
        Model instance;
        constexpr float vertices_data[] = {
                -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,
                0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f,
                -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
                -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
                0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
                0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,
                -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
                0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,
                -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f,
        };

        constexpr float texture_coord_data[] = {
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        };

        constexpr float normals_data[] = {
                0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
                0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
                -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
                -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
                1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
                0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
                0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        };

        instance.vertex_cnt = 36;
        instance.vertices = make_unique<float[]>(36 * 3);
        copy_n(vertices_data, 36 * 3, instance.vertices.get());
        instance.texture_coord = make_unique<float[]>(36 * 2);
        copy_n(texture_coord_data, 36 * 2, instance.texture_coord.get());
        instance.normals = make_unique<float[]>(36 * 3);
        copy_n(normals_data, 36 * 3, instance.normals.get());
        instance.position = randomVec3(-3, 0);
        glm::mat4 modelMat = glm::rotate(glm::mat4(1.0f),
                                         static_cast<float>(rand()) / RAND_MAX *
                                                 glm::radians(180.0f),
                                         randomVec3(0.0, 1.0));

        modelMat = glm::translate(modelMat, instance.position);
        instance.modelMat = modelMat;

        return instance;
    }

    static Model getTriangle() {
        Model instance;
        constexpr float vertices_data[] = {
                -0.5f, -0.5f, -0.5f, //
                0.5f,  -0.5f, -0.5f, //
                0.0f,  0.5f,  -0.5f,
        };

        constexpr float texture_coord_data[] = {0.0f, 0.0f, 1.0f,
                                                0.0f, 1.0f, 1.0f};

        constexpr float normals_data[] = {
                0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        };

        instance.vertex_cnt = 3;
        instance.vertices = make_unique<float[]>(3 * 3);
        copy_n(vertices_data, 3 * 3, instance.vertices.get());
        instance.texture_coord = make_unique<float[]>(3 * 2);
        copy_n(texture_coord_data, 3 * 2, instance.texture_coord.get());
        instance.normals = make_unique<float[]>(3 * 3);
        copy_n(normals_data, 3 * 3, instance.normals.get());
        instance.position = glm::vec3(0.0f, 0.0f, -0.5f);
        instance.modelMat = glm::mat4(1.0f);

        return instance;
    }
};
