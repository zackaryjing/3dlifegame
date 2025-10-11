#pragma once

#include <string>
#include <vector>
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
        sceneLists.push_back(new DemoScene(window));
        sceneCnt = 2;
    }

    static void render() {
        while (not glfwWindowShouldClose(window)) {
            sceneLists[renderIndex]->takeControl();
            sceneLists[renderIndex]->render();
        }
    }

    static void addCommonWidget() {
        if (Window::isAccessingUI) {
            auto changed = false;
            int lastIndex = -1;
            {
                ImGui::Begin("Config Window");
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
