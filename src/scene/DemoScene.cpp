#include <algorithm>
#include <format>
#include <vector>

#include <glad/glad.h>
// glad must be first
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "debug/Error.hpp"
#include "fonts/Font.hpp"
#include "group/Gizmo.hpp"
#include "group/Group.hpp"
#include "light/PointLight.hpp"
#include "model/Model.hpp"
#include "scene/DemoScene.hpp"
#include "scene/SceneManager.hpp"
#include "ui/CursorInput.hpp"
#include "ui/KeyboardInput.hpp"

string DemoScene::getText() {
    return std::format("Yaw:{:.2f} Pitch:{:.2f} Fov:{:.2f}", camera.yaw,
                       camera.pitch, camera.fov);
}
void DemoScene::addWidget(const float curTime, const ImGuiIO &io) {
    if (Window::isAccessingUI) {
        ImGui::Begin("Scene Window");

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Current animation count: %ld",
                    animationManager.currentAnimation.size());
        for (auto &light: lightManager.pointLights) {
            string label = "LightSpinning" + to_string(light->id);
            if (ImGui::Checkbox(label.c_str(), &light->lightTurning)) {
                if (light->lightTurning) {
                    light->resetAnimationTime(curTime);
                    animationManager.addAnimation(light->lightPosAnimation);
                    animationManager.addAnimation(light->lightColorAnimation);
                } else {
                    animationManager.removeAnimation(light->lightPosAnimation);
                    animationManager.removeAnimation(
                            light->lightColorAnimation);
                }
            }
        }
        int group_id = 0;
        for (auto &group: groups) {
            if (ImGui::Checkbox(format("ModelTurning {:d}", group_id++).c_str(),
                                &group.modelTurning)) {
                group.resetTime();
            }
        }
        ImGui::End();
    }
}

void DemoScene::render() {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    for (const auto &light: lightManager.pointLights) {
        if (light->lightTurning) {
            animationManager.addAnimation(light->lightPosAnimation);
            animationManager.addAnimation(light->lightColorAnimation);
        }
    }

    const auto &io = ImGui::GetIO();

    while (not glfwWindowShouldClose(window) and isRendering) {
        glfwPollEvents();
        glCheckError();
        GlobalKeyboard::processInput(window, deltaTime, camera);
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);

        const glm::mat4 view = camera.getView();
        glm::mat4 projection = camera.getProjection();

        const auto curTime = static_cast<float>(glfwGetTime());
        animationManager.update(curTime);
        lightManager.drawLights(view, projection);
        for (auto group: groups) {
            group.drawGroups(view, projection, lightManager, camera.cameraPos);
        }
        gizmo.drawGroups(view, camera.cameraPos);
        skybox.drawSkybox(view, projection);
        font.drawText(getText(), 25.0f, 25.0f, 2.0f,
                      glm::vec3(0.5f, 0.8f, 0.2f));


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Window::addCommonWidget(io);
        SceneManager::addCommonWidget();
        addWidget(curTime, io);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        deltaTime = curTime - lastFrame;
        lastFrame = curTime;

        glfwSwapBuffers(window);
        glfwSwapInterval(1);
    }
}

vector<float> DemoScene::genGLData() const {
    vector<shared_ptr<Model>> models = {};
    models.push_back(skybox.skyboxModel);
    for (auto light: lightManager.getModel()) {
        models.push_back(light);
    }

    for (auto group: groups) {
        for (auto model: group.modelGroup) {
            models.push_back(model);
        }
    }
    for (auto model: gizmo.modelGroup) {
        models.push_back(model);
    }

    const int total_cnts = ranges::fold_left(
            models, 0, [](const int total, const shared_ptr<Model> &model) {
                return total + model->vertexCnt;
            });

    vector<float> vertices;
    vertices.reserve(total_cnts * 8);
    int start_point = 0;
    for (const auto &model: models) {
        for (size_t i = 0; i < model->vertexCnt; ++i) {
            for (int j = 0; j < 3; ++j) {
                vertices.push_back(model->vertices[i * 3 + j]);
            }
            for (int j = 0; j < 2; ++j) {
                vertices.push_back(model->textureCoord[i * 2 + j]);
            }
            for (int j = 0; j < 3; ++j) {
                vertices.push_back(model->normals[i * 3 + j]);
            }
        }
        model->setDataPos(start_point, model->vertexCnt);
        start_point += model->vertexCnt;
    }
    return vertices;
}
