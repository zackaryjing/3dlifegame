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
using Type::VFVec2;
using Type::VFVec3;
using Type::VIVec3;


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
    Model(const Model &other) :
        vertices(std::make_unique<float[]>(other.vertex_cnt * 3)),
        texture_coord(std::make_unique<float[]>(other.vertex_cnt * 2)),
        normals(std::make_unique<float[]>(other.vertex_cnt * 3)),
        material_name(other.material_name), vertex_cnt(other.vertex_cnt),
        material(other.material), position(other.position),
        modelMat(other.modelMat), dataPos(other.dataPos) {

        std::copy_n(other.vertices.get(), vertex_cnt * 3, vertices.get());
        std::copy_n(other.texture_coord.get(), vertex_cnt * 2,
                    texture_coord.get());
        std::copy_n(other.normals.get(), vertex_cnt * 3, normals.get());
    }


    Model &operator=(const Model &other) {
        if (this != &other) {
            Model tmp(other);
            std::swap(*this, tmp);
        }
        return *this;
    }

    Model(Model &&) = default;
    Model &operator=(Model &&) = default;

    static Model genModel(VFVec3 vertices, const VIVec3 &faceIndices);

    static Model genModel(VFVec3 vertices, VFVec3 normals, VFVec2 texture_coord,
                          string material_name);

    static glm::vec3 randomVec3(const float min, const float max);

    static Model getCube();

    static Model getTriangle();
};
inline Model Model::genModel(VFVec3 vertices, const VIVec3 &faceIndices) {
    size_t n = faceIndices.size() * 3;
    VFVec3 fullVertices(n * 3);
    VFVec3 fullNormals(n * 3);
    VFVec2 fullTexture(n * 3);
    auto center = getAvgCenter(vertices);
    for (auto [index,face]: std::views::enumerate(faceIndices)) {
        auto vertexA = get<0>(face);
        auto vertexB = get<1>(face);
        auto vertexC = get<2>(face);
        fullVertices[index * 3] = vertices[vertexA];
        fullVertices[index * 3 + 1] = vertices[vertexB];
        fullVertices[index * 3 + 2] = vertices[vertexC];
        auto faceNormal = normalize(substract(
                getAvgCenter({fullVertices[index * 3], fullVertices[index * 3 + 1],
                              fullVertices[index * 3 + 2]}),
                center));
        fullNormals[index * 3] = faceNormal;
        fullNormals[index * 3 + 1] = faceNormal;
        fullNormals[index * 3 + 2] = faceNormal;
        fullTexture[index * 3] = {1.0, 1.0};
        fullTexture[index * 3 + 1] = {1.0, 1.0};
        fullTexture[index * 3 + 2] = {1.0, 1.0};
    }

    return genModel(fullVertices, fullNormals, fullTexture, "");
}
inline Model Model::genModel(VFVec3 vertices, VFVec3 normals,
                             VFVec2 texture_coord, string material_name) {
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
inline glm::vec3 Model::randomVec3(const float min, const float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution dis(min, max);
    return glm::vec3(dis(gen), dis(gen), dis(gen));
}
inline Model Model::getCube() {
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
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    };

    constexpr float normals_data[] = {
            0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,
            -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f,
            0.0f,  0.0f,  -1.0f, 0.0f,  0.0f, -1.0f, -1.0f, 0.0f, 0.0f,  -1.0f,
            0.0f,  0.0f,  -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
            0.0f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            0.0f,  0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f,
            0.0f,  0.0f,  -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
            0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f,
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
                                     static_cast<float>(rand()) /
                                             static_cast<float>(RAND_MAX) *
                                             glm::radians(180.0f),
                                     randomVec3(0.0, 1.0));

    modelMat = glm::translate(modelMat, instance.position);
    instance.modelMat = modelMat;

    return instance;
}
inline Model Model::getTriangle() {
    Model instance;
    constexpr float vertices_data[] = {
            -0.5f, -0.5f, -0.5f, //
            0.5f,  -0.5f, -0.5f, //
            0.0f,  0.5f,  -0.5f,
    };
    constexpr float texture_coord_data[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
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
