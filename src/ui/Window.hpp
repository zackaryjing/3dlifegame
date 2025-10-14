#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Window {
public:
    static inline int screenHeight;
    static inline int screenWidth;
    static inline int height;
    static inline int width;
    static inline float ratio = 0.8;
    static inline float aspect_ratio = 1.2;
    static inline const char *name = "LifeGame";
    static inline bool hasInit = false;
    static inline bool isAccessingUI = false;
    static inline bool show_demo_window = false;
    static inline ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);

    static void updateCursorState(GLFWwindow *window) {
        if (not isAccessingUI) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (glfwRawMouseMotionSupported()) {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            }
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            if (glfwRawMouseMotionSupported()) {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            }
        }
    }
    static inline void init() {
        if (not hasInit) {
            GLFWmonitor *primary = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(primary);
            screenWidth = mode->width;
            screenHeight = mode->height;
            height = static_cast<int>(ratio * static_cast<float>(screenHeight));
            width = static_cast<int>(ratio * static_cast<float>(screenWidth));
            aspect_ratio = static_cast<float>(screenWidth) /
                           static_cast<float>(screenHeight);
        }
    };

    static void commonWindowInit(const ImGuiIO &io) {
        (void) io;
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(1.2f);
        // style.FontScaleDpi = 1.2f;
    }

    static void addCommonWidget(const ImGuiIO &io) {
        (void) io;
        if (isAccessingUI) {

            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window);
            }


            //     ImGui::Checkbox("DemoWindow", &show_window);
            //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            //     ImGui::ColorEdit3("clear color",
            //                       reinterpret_cast<float *>(&clear_color));
            //     if (ImGui::Button("Button"))
            //         counter++;
            //     ImGui::SameLine();
        }
    }
    static void destoryCommonWindow() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};

inline void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
    (void) window;
    glViewport(0, 0, width, height);
}
