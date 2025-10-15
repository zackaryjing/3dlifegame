#pragma once

#include <string>
#include <vector>

#include "MagicCubeScene.hpp"
#include "scene/DemoScene.hpp"
#include "scene/Scene.hpp"
#include "ui/Window.hpp"

using std::string;
using std::vector;


class SceneManager {
    static inline vector<Scene *> sceneLists;
    static inline int renderIndex = 0;
    static inline GLFWwindow *window;

public:
    static inline int sceneCnt = 0;
    static void init(GLFWwindow *_window) {
        window = _window;
        sceneLists.push_back(new DemoScene(window, "Scene one"));
        sceneLists.push_back(new MagicCubeScene(window));
        sceneCnt = 2;
    }

    static void render() {
        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(
                glfwGetPrimaryMonitor());
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        Window::commonWindowInit(io);
        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        if (not ImGui_ImplGlfw_InitForOpenGL(window, false)) {
            cerr << "Failed to initialize ImGuiGlfwForOpenGL" << endl;
            return;
        }
        if (not ImGui_ImplOpenGL3_Init()) {
            cerr << "Failed to initialize ImGuiOpenGL3" << endl;
            return;
        }

        style.FontSizeBase = 18.0f;

        while (not glfwWindowShouldClose(window)) {
            sceneLists[renderIndex]->takeControl();
            sceneLists[renderIndex]->render();
        }

        Window::destoryCommonWindow();
    }

    static void addCommonWidget() {
        if (Window::isAccessingUI) {
            auto changed = false;
            int lastIndex = -1;
            {
                ImGui::Begin("Scene Selector");
                if (ImGui::TreeNode("Scene Select")) {
                    for (int i = 0; i < sceneCnt; i++) {
                        if (const auto &scene = sceneLists[i];
                            ImGui::Selectable(scene->name.c_str(),
                                              renderIndex == i))
                            if (i != renderIndex) {
                                lastIndex = renderIndex;
                                renderIndex = i;
                                changed = true;
                            }
                    }
                    ImGui::TreePop();
                }
                ImGui::End();
            }
            if (changed) {
                sceneLists[lastIndex]->stop();
            }
        }
    }
};
