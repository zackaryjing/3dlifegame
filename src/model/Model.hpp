// Add more models in one scene
#pragma once

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <random>
#include "model/Material.hpp"
#include "utils/Type.hpp"
using std::copy_n;
using std::make_pair;
using std::make_unique;
using std::pair;
using std::tuple;
using std::unique_ptr;


// model shouldn't have a render function.
// render a model is managed by the scene
class Model {
public:
    Model() = default;
    unique_ptr<float[]> vertices = {};
    unique_ptr<float[]> texture_coord = {};
    unique_ptr<float[]> normals = {};
    string material_name;
    size_t vertex_cnt = {};


    Material material;
    glm::vec3 position = {};
    glm::mat4 modelMat = {};
    /* data position in VAO */
    pair<int, int> dataPos;

    void setDataPos(int start, int cnt) { dataPos = make_pair(start, cnt); }

    static Model genModel(VFVec3 vertices, VFVec3 normals, VFVec2 texture_coord,
                          string material_name) {
        Model instance;
        size_t n = vertices.size();
        size_t m = normals.size();
        size_t l = texture_coord.size();
        if (n != m or m != l) {
            throw std::invalid_argument("params should be of the same length");
        }
        instance.vertices = make_unique<float[]>(n * 3);
        for (size_t i = 0; i < n; ++i) {
            auto vertex = vertices[i];
            instance.vertices[i * 3] = get<0>(vertex);
            instance.vertices[i * 3 + 1] = get<1>(vertex);
            instance.vertices[i * 3 + 2] = get<2>(vertex);
        }
        instance.normals = make_unique<float[]>(n * 3);
        for (size_t i = 0; i < n; ++i) {
            auto normal = normals[i];
            instance.normals[i * 3] = get<0>(normal);
            instance.normals[i * 3 + 1] = get<1>(normal);
            instance.normals[i * 3 + 2] = get<2>(normal);
        }
        instance.texture_coord = make_unique<float[]>(n * 2);
        for (size_t i = 0; i < n; ++i) {
            auto coord = texture_coord[i];
            instance.texture_coord[i * 2] = get<0>(coord);
            instance.texture_coord[i * 2 + 1] = get<1>(coord);
        }
        instance.vertex_cnt = n;
        instance.material_name = material_name;
        instance.modelMat = glm::mat4(1.0f);
        instance.position = glm::vec3(0.0f, 0.0f, 0.0f);
        return instance;
    }

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
