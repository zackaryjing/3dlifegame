#pragma once
#include <glm/glm.hpp>
#include "light/Light.hpp"
#include "model/Model.hpp"
#include "model/ModelLoader.hpp"

using std::make_shared;
using std::shared_ptr;
using std::string;

class Group {
    Shader groupShader;

    static inline string vertex_shader = GLSL_DIR "CubeShader.vs";
    static inline string fragment_shader = GLSL_DIR "CubeShader.fs";
    const bool modelTurning = true;
    unsigned int groupVAO;

public:
    vector<shared_ptr<Model>> modelGroup;
    Group() :
        groupShader(vertex_shader, fragment_shader, ShaderParamType::PATH) {
        glGenVertexArrays(1, &groupVAO);
        glBindVertexArray(groupVAO);
    }
    void addModel(shared_ptr<Model> model) { modelGroup.push_back(model); };

    void setCommonUni(glm::mat4 view, glm::mat4 projection, Light &light) {
        groupShader.setInt("ourTexture", 0);
        groupShader.setMatrix4("view", view);
        groupShader.setMatrix4("projection", projection);
        groupShader.setVec3("viewPos", Camera::cameraPos);
        groupShader.setVec3("light.position", light.position);
        groupShader.setVec3("light.ambient", light.ambientColor);
        groupShader.setVec3("light.diffuse", light.diffuseColor);
        groupShader.setVec3("light.specular", light.specularColor);
        groupShader.setVec3("light.color", light.lightColor);
    }

    void setModelUni(shared_ptr<Model> model) {
        groupShader.setVec3("material.color", model->material.color);
        groupShader.setVec3("material.ambient", model->material.ambient);
        groupShader.setVec3("material.diffuse", model->material.diffuse);
        groupShader.setVec3("material.specular", model->material.specular);
        groupShader.setFloat("material.shininess", model->material.shininess);
    }

    void drawGroups(glm::mat4 view, glm::mat4 projection, Light &light) {
        groupShader.use();
        setCommonUni(view, projection, light);
        for (auto model: modelGroup) {
            setModelUni(model);

            auto modelMat = glm::mat4(1.0f);
            if (modelTurning) {
                modelMat = glm::rotate(model->modelMat,
                                       static_cast<float>(glfwGetTime()) *
                                               glm::radians(50.0f),
                                       glm::vec3(0.5f, 1.0f, 0.0f));
            } else {
                modelMat = model->modelMat;
            }
            groupShader.setMatrix4("model", modelMat);
            glDrawArrays(GL_TRIANGLES, model->dataPos.first,
                         model->dataPos.second);
        }
    }

    static inline Group getGroup() {
        Group group;
        for (int i = 0; i < 5; ++i) {
            group.modelGroup.emplace_back(make_shared<Model>(Model::getCube()));
        }

        string model_dir = MODEL_DIR "arrow.obj";
        group.addModel(make_shared<Model>(ModelLoader::loadModel(model_dir)));
        return group;
    }
};
