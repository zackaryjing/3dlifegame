#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include "group/Group.hpp"

class Gizmo : public Group {
protected:
    static inline string vertex_shader = GLSL_DIR "GizmoShader.vs";
    static inline string fragment_shader = GLSL_DIR "GizmoShader.fs";
    float halfHeight = 1.0f;
    float halfWidth = halfHeight * Window::aspect_ratio;
    Shader gizmoShader;
    static inline float posRatio = 0.8f;
    const glm::mat4 projection = glm::translate(
            glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight),
            glm::vec3(halfWidth *posRatio, halfHeight *posRatio, -0.1f));

public:
    Gizmo() :
        gizmoShader(vertex_shader, fragment_shader, ShaderParamType::PATH,
                    "gizmoShader") {
        string model_dir = MODEL_DIR "arrow.obj";
        addModel(make_shared<Model>(ModelLoader::loadModel(model_dir)));
        addModel(make_shared<Model>(ModelLoader::loadModel(model_dir)));
        addModel(make_shared<Model>(ModelLoader::loadModel(model_dir)));
        auto modelx = glm::mat4(1.0f), modely = glm::mat4(1.0f),
             modelz = glm::mat4(1.0f);
        modelx = glm::rotate(modelx, glm::radians(90.0f),
                             glm::vec3(1.0f, 0.0f, 0.0f));
        modelz = glm::rotate(modelz, glm::radians(-90.0f),
                             glm::vec3(0.0f, 0.0f, 1.0f));
        modelx = glm::scale(modelx, glm::vec3(0.1f, 0.1f, 0.1f));
        modely = glm::scale(modely, glm::vec3(0.1f, 0.1f, 0.1f));
        modelz = glm::scale(modelz, glm::vec3(0.1f, 0.1f, 0.1f));
        modelGroup[0]->modelMat = modelx; // pointing front
        modelGroup[1]->modelMat = modely; // pointing up
        modelGroup[2]->modelMat = modelz; // pointing right
        modelGroup[0]->material.setAllSame(glm::vec3(1.0f, 0.0f, 0.0f));
        modelGroup[1]->material.setAllSame(glm::vec3(0.0f, 0.0f, 1.0f));
        modelGroup[2]->material.setAllSame(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void setCommonUniform(const glm::mat4 view) {
        auto rotation = glm::mat4(glm::mat3(view));
        gizmoShader.setInt("ourTexture", 0);
        gizmoShader.setMatrix4("view", rotation);
        gizmoShader.setMatrix4("projection", projection);
        gizmoShader.setVec3("viewPos", Camera::cameraPos);
        gizmoShader.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
        gizmoShader.setVec3("light.color", glm::vec3(0.8f, 0.8f, 0.8f));
    }

    void setGizmoUniform(const shared_ptr<Model> &model) {
        gizmoShader.setVec3("material.color", model->material.color);
        gizmoShader.setVec3("material.ambient", model->material.ambient);
        gizmoShader.setMatrix4("model", model->modelMat);
    }
    void drawGroups(const glm::mat4 &view) {
        gizmoShader.use();
        setCommonUniform(view);
        for (auto model: modelGroup) {
            setGizmoUniform(model);
            glDrawArrays(GL_TRIANGLES, model->dataPos.first,
                         model->dataPos.second);
        }
    }
};
