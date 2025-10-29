#pragma once
#include <glad/glad.h>
#include <iostream>

#include "light/Light.hpp"
#include "model/Model.hpp"
#include "model/ModelLoader.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;

class DirLight : public Light {
public:
    DirLight(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
             const glm::vec3 &specularColor, const glm::vec3 &lightColor,
             const glm::vec3 &pos, const glm::vec3 &dir);
    DirLight(const glm::vec3 &color, const glm::vec3 &position,
             const glm::vec3 &direction);
    DirLight();
    glm::vec3 direction;
    void setModel() override;

    void setCommonUniform(const glm::mat4 &view,
                          const glm::mat4 &projection) const {
        lightShader.setInt("ourTexture", 0);
        lightShader.setMatrix4("view", view);
        lightShader.setMatrix4("projection", projection);
    }

    void setLightUniform() const {
        lightShader.setVec3("light.position", position);
        lightShader.setVec3("light.ambient", ambientColor);
        lightShader.setVec3("light.diffuse", diffuseColor);
        lightShader.setVec3("light.specular", specularColor);
        lightShader.setVec3("light.color", lightColor);
    }

    void useLightUniform(const Shader &shader) override {
        shader.setVec3("dirLight.direction", direction);
        shader.setVec3("dirLight.ambient", ambientColor);
        shader.setVec3("dirLight.diffuse", diffuseColor);
        shader.setVec3("dirLight.specular", specularColor);
    }

    void drawLight(glm::mat4 view, glm::mat4 projection) override {
        lightShader.use();
        setCommonUniform(view, projection);
        setLightUniform();
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightShader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, lightModel->dataPos.first,
                     lightModel->dataPos.second);
    }
};

inline DirLight::DirLight(const glm::vec3 &ambientColor,
                          const glm::vec3 &diffuseColor,
                          const glm::vec3 &specularColor,
                          const glm::vec3 &lightColor, const glm::vec3 &pos,
                          const glm::vec3 &dir) :
    Light(ambientColor, diffuseColor, specularColor, lightColor, pos),
    direction(dir) {
    DirLight::setModel();
}


inline DirLight::DirLight(const glm::vec3 &color, const glm::vec3 &position,
                          const glm::vec3 &dir) :
    DirLight(color, color, color, color, position, dir) {}

inline DirLight::DirLight() :
    DirLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
             glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)) {}


inline void DirLight::setModel() {
    string model_dir = MODEL_DIR "arrow.obj";
    const auto arrow = ModelLoader::loadModel(model_dir);
    lightModel = make_shared<Model>(arrow);
    lightModel->modelMat = glm::translate(lightModel->modelMat, position);
}
