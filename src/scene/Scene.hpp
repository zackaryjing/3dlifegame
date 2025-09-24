#pragma once
#include <algorithm>
#include <format>
#include <limits>
#include <numeric>
#include <ranges>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
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
#include "texture/TextureLoader.hpp"
#include "ui/CursorInput.hpp"
#include "ui/KeyboardInput.hpp"

using std::vector;


class Scene {
public:
    vector<Group> groups;
    Light light;
    FontPixelStyle font;
    Gizmo gizmo;
    Camera camera;
    Cursor cursor;
    KeyboardMoveControl keyboard_move_control;
    vector<float> genGLData();
    unsigned int putDataToGL();
    unsigned int VAO;
    unsigned int VBO;
    GLFWwindow *window;

    void takeControl() {
        GlobalCursor::setCursor(&cursor);
        GlobalKeyboard::setKeyboard(&keyboard_move_control);
        glfwSetCursorPosCallback(window, GlobalCursor::mouse_callback);
        glfwSetScrollCallback(window, GlobalCursor::scroll_callback);
        glfwSetWindowFocusCallback(window, GlobalCursor::window_focus_callback);
    }

    Scene(GLFWwindow *window) : cursor(camera), window(window) {
        groups.push_back(Group::getDemoGroup(5));
        glGenBuffers(1, &VBO);
        VAO = putDataToGL();
        takeControl();
    }

    string getText() {
        return std::format("Yaw:{:.2f} Pitch:{:.2f} Fov:{:.2f}", camera.yaw,
                           camera.pitch, camera.fov);
    }

    void addWidget() {
        if (Window::isAccessingUI) {
            ImGui::Begin("Scene Window");
            ImGui::Checkbox("LightSpinning", &light.lightTurning);
            for (auto &group: groups) {
                ImGui::Checkbox("ModelTurning", &group.modelTurning);
            }
            ImGui::End();
        }
    }

    void render() {
        glEnable(GL_DEPTH_TEST);
        unsigned int texture = create_brick_wall_texture();
        glActiveTexture(GL_TEXTURE0); // activate texture unit first
        glBindTexture(GL_TEXTURE_2D, texture);


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        Window::commonWindowInit(io);
        ImGui::StyleColorsDark();

        if (not ImGui_ImplGlfw_InitForOpenGL(window, true)) {
            cerr << "Failed to initialize ImGuiGlfwForOpenGL" << endl;
            return;
        }
        if (not ImGui_ImplOpenGL3_Init()) {
            cerr << "Failed to initialize ImGuiOpenGL3" << endl;
            return;
        }


        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while (not glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glCheckError();
            GlobalKeyboard::processInput(window, deltaTime, camera);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
            addWidget();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            const auto currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwSwapBuffers(window);
            glfwSwapInterval(1);
        }

        Window::destoryCommonWindow();
    }
};


inline unsigned int Scene::putDataToGL() {
    constexpr int stride = 8;
    // vertices
    constexpr GLuint vertices_index = 0;
    constexpr GLint vertices_size = 3;
    constexpr GLsizei vertices_stride = stride * sizeof(float);
    const auto vertices_pointer = reinterpret_cast<void *>(0 * sizeof(float));

    // texture coordinate
    constexpr GLuint coord_index = 1;
    constexpr GLint coord_size = 2;
    constexpr GLsizei coord_stride = stride * sizeof(float);
    const auto coord_pointer = reinterpret_cast<void *>(3 * sizeof(float));

    // normals
    constexpr GLuint normals_index = 2;
    constexpr GLint normals_size = 3;
    constexpr GLsizei normals_stride = stride * sizeof(float);
    const auto normals_pointer = reinterpret_cast<void *>(5 * sizeof(float));

    // create and bind vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // create vertex buffer object
    const auto vertices = genGLData();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<long>(sizeof(float) * vertices.size()),
                 vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(vertices_index, vertices_size, GL_FLOAT, GL_FALSE,
                          vertices_stride, vertices_pointer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(coord_index, coord_size, GL_FLOAT, GL_FALSE,
                          coord_stride, coord_pointer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(normals_index, normals_size, GL_FLOAT, GL_FALSE,
                          normals_stride, normals_pointer);
    glEnableVertexAttribArray(2);
    return VAO;
}

inline vector<float> Scene::genGLData() {
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
