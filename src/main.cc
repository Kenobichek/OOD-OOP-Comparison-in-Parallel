#include <glad/glad.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

int main() {
    // Инициализация GLFW
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Включить вертикальную синхронизацию

    // Инициализация glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Инициализация Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Цикл отрисовки
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Начало фрейма ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Создание интерфейса
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        if (ImGui::Button("Click me!"))
            std::cout << "Button clicked!\n";
        ImGui::End();

        // Рендеринг интерфейса ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Обновление окна
        glfwSwapBuffers(window);
    }

    // Очистка Dear ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Очистка GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
