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
#include "model/Model.hpp"
#include "scene/SceneManager.hpp"
#include "ui/CursorInput.hpp"
#include "ui/KeyboardInput.hpp"

string MagicCubeScene::getText(const ImGuiIO &io) {
    return std::format("Yaw:{:.2f} Pitch:{:.2f} Fov:{:.2f} Fps: {:}",
                       camera.yaw, camera.pitch, camera.fov, io.Framerate);
}
void MagicCubeScene::addWidget(const float curTime) {
    ImGui::Begin("Scene Window");
    static char text[1024 * 2] = "R F B2 U2 D F' R' L' F2 U2 R' D' B"
                                 " B' D R U2 F2 L R F D' U2 B2 F' R'";
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput |
                                       ImGuiInputTextFlags_CtrlEnterForNewLine;
    ImGui::InputTextMultiline(
            "##source", text, IM_ARRAYSIZE(text),
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 1.5), flags);
    static string msg;
    if (ImGui::Button("Execute")) {
        magicCube.executeInput(string(text), msg, curTime);
    }
    if (ImGui::Button("Reset")) {
        magicCube.reset();
    }
    ImGui::Text("%s", msg.c_str());

    ImGui::End();
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

        const auto curTime = static_cast<float>(glfwGetTime());
        animationManager.update(curTime);
        lightManager.drawLights(view, projection);
        for (auto group: groups) {
            group.drawGroups(view, projection, lightManager, camera.cameraPos);
        }
        gizmo.drawGroups(view, camera.cameraPos);
        font.drawText(getText(io), 25.0f, 25.0f, 2.0f,
                      glm::vec3(0.5f, 0.8f, 0.2f));


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Window::addCommonWidget(io);
        SceneManager::addCommonWidget();
        addWidget(curTime);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        deltaTime = curTime - lastFrame;
        lastFrame = curTime;

        glfwSwapBuffers(window);
        glfwSwapInterval(1);
    }
}

vector<float> MagicCubeScene::genGLData() const {
    vector<shared_ptr<Model>> models = {};
    for (const auto &light: lightManager.getModel()) {
        models.push_back(light);
    }

    for (const auto &group: groups) {
        for (auto model: group.modelGroup) {
            models.push_back(model);
        }
    }
    for (const auto &model: gizmo.modelGroup) {
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
