#pragma once
#include <format>
#include <vector>

#include <GLFW/glfw3.h>

#include "animation/AnimationManager.hpp"
#include "fonts/Font.hpp"
#include "group/Gizmo.hpp"
#include "group/Group.hpp"
#include "light/Light.hpp"
#include "logic/MagicCube.hpp"
#include "scene/Scene.hpp"
#include "skybox/Skybox.hpp"
#include "ui/CursorInput.hpp"
#include "ui/KeyboardInput.hpp"

using std::vector;


class DemoScene final : public Scene {
public:
    Light light;
    FontPixelStyle font;
    Gizmo gizmo;
    Camera camera;
    Cursor cursor;
    AnimationManager animationManager;
    KeyboardMoveControl keyboard_move_control;
    Skybox skybox;
    [[nodiscard]] vector<float> genGLData() const override;
    GLFWwindow *window;
    bool isRendering = false;

    void takeControl() override {
        isRendering = true;
        GlobalCursor::setCursor(&cursor);
        GlobalKeyboard::setKeyboard(&keyboard_move_control);
        glfwSetCursorPosCallback(window, GlobalCursor::mouse_callback);
        glfwSetScrollCallback(window, GlobalCursor::scroll_callback);
        glfwSetWindowFocusCallback(window, GlobalCursor::window_focus_callback);
        glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
        glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
    }

    explicit DemoScene(GLFWwindow *window) :
        Scene(), cursor(camera), window(window) {
        name = "Default Name";
        groups.push_back(Group::getDemoGroup(5));
        glGenBuffers(1, &VBO);
        Scene::putDataToGL();
        DemoScene::takeControl();
    }

    DemoScene(GLFWwindow *window, const string &name) : DemoScene(window) {
        this->name = name;
    }
    string getText();
    void addWidget(float curTime, const ImGuiIO &io);
    void render() override;
    void stop() override { isRendering = false; }
};
