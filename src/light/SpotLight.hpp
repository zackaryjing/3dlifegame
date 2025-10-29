#pragma once
#include <glad/glad.h>
#include <iostream>


#include "animation/Animation.hpp"
#include "camera/Camera.hpp"
#include "light/Light.hpp"
#include "model/Model.hpp"
#include "model/ModelLoader.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;

class SpotLight : public Light {
public:
    SpotLight(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
              const glm::vec3 &specularColor, const glm::vec3 &lightColor,
              const glm::vec3 &pos, const glm::vec3 &dir, float cutOff,
              float outerCutOff);
    SpotLight(const glm::vec3 &color, const glm::vec3 &position,
              const glm::vec3 &direction, float cutOff, float outerCutOff);
    SpotLight();

    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
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
        shader.setVec3("spotLight.position", position);
        shader.setVec3("spotLight.direction", direction);
        shader.setVec3("spotLight.ambient", ambientColor);
        shader.setVec3("spotLight.diffuse", diffuseColor);
        shader.setVec3("spotLight.specular", specularColor);
        shader.setFloat("spotLight.cutOff", cutOff);
        shader.setFloat("spotLight.outerCutOff", outerCutOff);
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

inline SpotLight::SpotLight(const glm::vec3 &ambientColor,
                            const glm::vec3 &diffuseColor,
                            const glm::vec3 &specularColor,
                            const glm::vec3 &lightColor, const glm::vec3 &pos,
                            const glm::vec3 &dir, const float cutOff,
                            const float outerCutOff) :
    Light(ambientColor, diffuseColor, specularColor, lightColor, pos),
    direction(dir), cutOff(cutOff), outerCutOff(outerCutOff) {
    SpotLight::setModel();
}


inline SpotLight::SpotLight(const glm::vec3 &color, const glm::vec3 &position,
                            const glm::vec3 &direction, const float cutOff,
                            const float outerCutOff) :
    SpotLight(color, color, color, color, position, direction, cutOff,
              outerCutOff) {}

inline SpotLight::SpotLight() :
    SpotLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
              glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
              glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 12.5,
              17.5) {}

inline void SpotLight::setModel() {
    string model_dir = MODEL_DIR "arrow.obj";
    const auto arrow = ModelLoader::loadModel(model_dir);
    lightModel = make_shared<Model>(arrow);
    lightModel->modelMat = glm::translate(lightModel->modelMat, position);
}
