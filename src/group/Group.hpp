#pragma once
#include <glm/glm.hpp>

#include "geometry/Icosahedron.hpp"
#include "geometry/Sphere.hpp"
#include "light/Light.hpp"
#include "model/Model.hpp"
#include "model/ModelLoader.hpp"

using std::make_shared;
using std::shared_ptr;
using std::string;

class Group {
protected:
    Shader groupShader;

    static inline string vertex_shader = GLSL_DIR "CubeShader.vs";
    static inline string fragment_shader = GLSL_DIR "CubeShader.fs";
    unsigned int groupVAO;

public:
    string groupName = "";
    bool modelTurning = false;
    vector<shared_ptr<Model>> modelGroup;
    Group() :
        groupShader(vertex_shader, fragment_shader, ShaderParamType::PATH,
                    "groupShader") {}

    explicit Group(const Shader shader) : groupShader(shader) {}

    void addModel(shared_ptr<Model> model) { modelGroup.push_back(model); };

    void setCommonUniform(glm::mat4 view, glm::mat4 projection, Light &light,
                          glm::vec3 cameraPos) {
        groupShader.setMatrix4("view", view);
        groupShader.setMatrix4("projection", projection);
        groupShader.setVec3("viewPos", cameraPos);
        groupShader.setVec3("light.position", light.position);
        groupShader.setVec3("light.ambient", light.ambientColor);
        groupShader.setVec3("light.diffuse", light.diffuseColor);
        groupShader.setVec3("light.specular", light.specularColor);
        groupShader.setVec3("light.color", light.lightColor);
    }

    void setModelUniform(shared_ptr<Model> model) {
        groupShader.setVec3("material.color", model->material.color);
        groupShader.setVec3("material.specular", model->material.specular);
        groupShader.setFloat("material.shininess", model->material.shininess);
        groupShader.setBool("material.useTexture", model->use_texture);
        groupShader.setInt("material.texDiffuse", 1);
    }

    void drawGroups(const glm::mat4 &view, const glm::mat4 &projection,
                    Light &light, const glm::vec3 cameraPos) {
        groupShader.use();
        setCommonUniform(view, projection, light, cameraPos);
        for (const auto &model: modelGroup) {
            setModelUniform(model);

            glm::mat4 modelMat;
            if (modelTurning) {
                modelMat = glm::rotate(model->modelMat,
                                       static_cast<float>(glfwGetTime()) *
                                               glm::radians(50.0f),
                                       glm::vec3(0.5f, 1.0f, 0.0f));
            } else {
                modelMat = model->modelMat;
            }
            groupShader.setMatrix4("model", modelMat);
            if (model->use_texture) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, model->diffuse_textureID);
            }
            glDrawArrays(GL_TRIANGLES, model->dataPos.first,
                         model->dataPos.second);
        }
    }

    static inline Group getDemoGroup(int n) {
        Group group;
        group.groupName = "DemoGroup";
        group.modelTurning = false;
        for (int i = 0; i < n; ++i) {
            group.modelGroup.emplace_back(make_shared<Model>(Model::getCube()));
        }
        group.modelTurning = true;

        // Icosahedron icosahedron(1.0);
        Sphere sphere(1.0);
        group.modelGroup.push_back(make_shared<Model>(sphere.toModel()));
        // group.modelGroup.push_back(make_shared<Model>(Model::getWoodenBox()));
        group.modelGroup.push_back(make_shared<Model>(Model::getTriangle()));
        // group.modelGroup.push_back(make_shared<Model>(icosahedron.toModel()));

        // string model_dir = MODEL_DIR "arrow.obj";
        // group.modelGroup.push_back(
        //         make_shared<Model>(ModelLoader::loadModel(model_dir)));

        return group;
    }
};
