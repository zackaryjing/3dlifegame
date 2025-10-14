#pragma once
#include <glad/glad.h>
//
#include <GLFW/glfw3.h>
#include <vector>

#include "animation/AnimationManager.hpp"
#include "fonts/Font.hpp"
#include "group/Gizmo.hpp"
#include "group/Group.hpp"
#include "light/Light.hpp"
#include "logic/MagicCube.hpp"
#include "scene/Scene.hpp"
#include "ui/CursorInput.hpp"
#include "ui/KeyboardInput.hpp"


class MagicCubeScene final : public Scene {
public:
    vector<Group> groups;
    Light light = Light({0.7f, 0.7f, 0.7f}, {0.0f, 5.0f, 0.0f});
    FontPixelStyle font;
    Gizmo gizmo;
    Camera camera;
    Cursor cursor;
    AnimationManager animationManager;
    MagicCube magicCube = MagicCube(animationManager);
    KeyboardMoveControl keyboard_move_control;
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

    explicit MagicCubeScene(GLFWwindow *window) :
        Scene(), cursor(camera), window(window) {
        name = "Magic Cube Scene";
        groups.push_back(magicCube.toGroup());
        glGenBuffers(1, &VBO);
        Scene::putDataToGL();
        MagicCubeScene::takeControl();
    }

    MagicCubeScene(GLFWwindow *window, const string &name) :
        MagicCubeScene(window) {
        this->name = name;
    }
    string getText(const ImGuiIO &io);
    void addWidget(float curTime);
    void render() override;
    void stop() override { isRendering = false; }
};
