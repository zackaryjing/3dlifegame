#pragma once

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <random>
#include "model/Material.hpp"
#include "texture/TextureLoader.hpp"
#include "utils/Type.hpp"
using std::copy_n;
using std::make_pair;
using std::pair;
using std::tuple;
using Type::VFVec2;
using Type::VFVec3;
using Type::VIVec3;


enum class ModelNormalGenType { FACEBASED, VERTEXBASED };

// model shouldn't have a render function.
// render a model is managed by the scene
class Model {
public:
    Model() = default;
    vector<float> vertices = {};
    vector<float> texture_coord = {};
    vector<float> normals = {};
    string material_name;
    size_t vertex_cnt = {};
    Material material;
    glm::vec3 position = {};
    glm::mat4 modelMat = {};
    /* data position in VAO */
    pair<int, int> dataPos;
    bool use_texture = false;
    unsigned int diffuse_textureID = 0;


    void setDataPos(int start, int cnt) { dataPos = make_pair(start, cnt); }

    Model(Model &&) = default;
    Model &operator=(Model &&) = default;
    Model(const Model &other) = default;
    Model &operator=(const Model &other) = default;

    static Model genModel(VFVec3 vertices, const VIVec3 &faceIndices,
                          ModelNormalGenType model_normal_gen);

    static Model genModel(VFVec3 vertices, VFVec3 normals, VFVec2 texture_coord,
                          string material_name);

    static glm::vec3 randomVec3(float min, float max);

    static Model getCube();

    static Model getWoodenBox();

    static Model getTriangle();
};


inline Model Model::genModel(VFVec3 vertices, const VIVec3 &faceIndices,
                             const ModelNormalGenType model_normal_gen =
                                     ModelNormalGenType::FACEBASED) {
    size_t n = faceIndices.size() * 3;
    VFVec3 fullVertices(n * 3);
    VFVec3 fullNormals(n * 3);
    VFVec2 fullTexture(n * 3);
    auto center = getAvgCenter(vertices);
    for (auto [index, face]: std::views::enumerate(faceIndices)) {
        auto vertexA = vertices[get<0>(face)];
        auto vertexB = vertices[get<1>(face)];
        auto vertexC = vertices[get<2>(face)];
        fullVertices[index * 3] = vertexA;
        fullVertices[index * 3 + 1] = vertexB;
        fullVertices[index * 3 + 2] = vertexC;
        auto faceNormal =
                normalize(substract(getAvgCenter({fullVertices[index * 3],
                                                  fullVertices[index * 3 + 1],
                                                  fullVertices[index * 3 + 2]}),
                                    center));
        if (model_normal_gen == ModelNormalGenType::FACEBASED) {
            fullNormals[index * 3] = faceNormal;
            fullNormals[index * 3 + 1] = faceNormal;
            fullNormals[index * 3 + 2] = faceNormal;
        } else if (model_normal_gen == ModelNormalGenType::VERTEXBASED) {
            fullNormals[index * 3] = normalize(substract(vertexA, center));
            fullNormals[index * 3 + 1] = normalize(substract(vertexB, center));
            fullNormals[index * 3 + 2] = normalize(substract(vertexC, center));
        }
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
    instance.vertices = vector<float>(n * 3);
    for (size_t i = 0; i < n; ++i) {
        auto vertex = vertices[i];
        instance.vertices[i * 3] = get<0>(vertex);
        instance.vertices[i * 3 + 1] = get<1>(vertex);
        instance.vertices[i * 3 + 2] = get<2>(vertex);
    }
    instance.normals = vector<float>(n * 3);
    for (size_t i = 0; i < n; ++i) {
        auto normal = normals[i];
        instance.normals[i * 3] = get<0>(normal);
        instance.normals[i * 3 + 1] = get<1>(normal);
        instance.normals[i * 3 + 2] = get<2>(normal);
    }
    instance.texture_coord = vector<float>(n * 2);
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
    const vector vertices_data = {
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

    const vector texture_coord_data = {
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    };

    const vector normals_data = {
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
    instance.vertices = vertices_data;
    instance.texture_coord = texture_coord_data;
    instance.normals = normals_data;
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


inline Model Model::getWoodenBox() {
    auto baseCube = getCube();
    baseCube.use_texture = true;
    baseCube.diffuse_textureID = create_wooden_box_texture();
    return baseCube;
}


inline Model Model::getTriangle() {
    Model instance;
    const vector vertices_data = {
            -0.5f, -0.5f, -0.5f, //
            0.5f,  -0.5f, -0.5f, //
            0.5f,  0.5f,  -0.5f,
    };
    const vector texture_coord_data = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    const vector normals_data = {
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    instance.vertex_cnt = 3;
    instance.vertices = vertices_data;
    instance.texture_coord = texture_coord_data;
    instance.normals = normals_data;
    instance.position = glm::vec3(0.0f, 2.0f, -2.0f);
    instance.modelMat = glm::translate(glm::mat4(1.0f), instance.position);
    instance.use_texture = true;
    instance.diffuse_textureID = create_wooden_box_texture();

    return instance;
}
