#include <algorithm>
#include <format>
#include <vector>

#include <glad/glad.h>
// glad must be first
#include "MagicCubeScene.hpp"


#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "debug/Error.hpp"
#include "fonts/Font.hpp"
#include "group/Gizmo.hpp"
#include "group/Group.hpp"
#include "light/Light.hpp"
#include "model/Model.hpp"
#include "scene/SceneManager.hpp"
#include "ui/CursorInput.hpp"
#include "ui/KeyboardInput.hpp"

string MagicCubeScene::getText() {
    return std::format("Yaw:{:.2f} Pitch:{:.2f} Fov:{:.2f}", camera.yaw,
                       camera.pitch, camera.fov);
}
void MagicCubeScene::addWidget() {
    if (Window::isAccessingUI) {
        ImGui::Begin("Scene Window");
        if (ImGui::Checkbox("LightSpinning", &light.lightTurning)) {
            light.resetTime();
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

void MagicCubeScene::render() {
    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    const auto &io = ImGui::GetIO();

    while (not glfwWindowShouldClose(window) and isRendering) {
        glfwPollEvents();
        glCheckError();
        GlobalKeyboard::processInput(window, deltaTime, camera);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);

        const glm::mat4 view = camera.getView();
        glm::mat4 projection = camera.getProjection();

        light.drawLight(view, projection);
        for (auto group: groups) {
            group.drawGroups(view, projection, light, camera.cameraPos);
        }
        gizmo.drawGroups(view, camera.cameraPos);
        font.drawText(getText(), 25.0f, 25.0f, 2.0f,
                      glm::vec3(0.5f, 0.8f, 0.2f));


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Window::addCommonWidget(io);
        SceneManager::addCommonWidget();
        addWidget();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwSwapBuffers(window);
        glfwSwapInterval(1);
    }
}

vector<float> MagicCubeScene::genGLData() const {
    vector<shared_ptr<Model>> models = {};
    models.push_back(light.lightModel);

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
                return total + model->vertex_cnt;
            });

    vector<float> vertices;
    vertices.reserve(total_cnts * 8);
    int start_point = 0;
    for (const auto &model: models) {
        for (size_t i = 0; i < model->vertex_cnt; ++i) {
            for (int j = 0; j < 3; ++j) {
                vertices.push_back(model->vertices[i * 3 + j]);
            }
            for (int j = 0; j < 2; ++j) {
                vertices.push_back(model->texture_coord[i * 2 + j]);
            }
            for (int j = 0; j < 3; ++j) {
                vertices.push_back(model->normals[i * 3 + j]);
            }
        }
        model->setDataPos(start_point, model->vertex_cnt);
        start_point += model->vertex_cnt;
    }
    return vertices;
}
