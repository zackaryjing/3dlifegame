#pragma once
#include <iostream>

#include <glad/glad.h>
#include <iostream>

#include "animation/Animation.hpp"
#include "camera/Camera.hpp"
#include "model/Model.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;


class Skybox {
public:
    Skybox();
    string skyboxFshader = GLSL_DIR "SkyboxShader.fs";
    string skyboxVshader = GLSL_DIR "SkyboxShader.vs";
    Shader skyboxShader = {};
    vector<string> faces = {
            TEXTURE_DIR "skybox/right.jpg", TEXTURE_DIR "skybox/left.jpg",
            TEXTURE_DIR "skybox/top.jpg",   TEXTURE_DIR "skybox/bottom.jpg",
            TEXTURE_DIR "skybox/front.jpg", TEXTURE_DIR "skybox/back.jpg",

    };
    unsigned int cubemapTexture = 0;
    shared_ptr<Model> skyboxModel;

    void setCommonUniform(const glm::mat4 &view,
                          const glm::mat4 &projection) const {
        skyboxShader.setMatrix4("view", view);
        skyboxShader.setMatrix4("projection", projection);
        skyboxShader.setInt("skybox", 3);
    }

    void drawSkybox(const glm::mat4 &view, const glm::mat4 &projection) const {
        skyboxShader.use();
        setCommonUniform(glm::mat4(glm::mat3(view)), projection);
        glDrawArrays(GL_TRIANGLES, skyboxModel->dataPos.first,
                     skyboxModel->dataPos.second);
    }
};

inline Skybox::Skybox() {
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    skyboxShader = Shader(skyboxVshader, skyboxFshader, ShaderParamType::PATH);
    skyboxModel = make_shared<Model>(Model::getCube());
    cubemapTexture = loadCubeMap(faces);
}
