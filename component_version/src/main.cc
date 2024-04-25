#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

class Entity {
public:
    std::vector<Position> positions;
    std::vector<Velocity> velocities;
};

class MovementSystem {
    public:
        void update(Entity& entity) {
            for (size_t i = 0; i < entity.positions.size(); ++i) {
                entity.positions[i].x += entity.velocities[i].dx;
                entity.positions[i].y += entity.velocities[i].dy;
            }
        }
};

class CircleRenderingSystem {
    public:
        void drawCircle(float x, float y, float radius) {
            const int num_segments = 30;
            glBegin(GL_TRIANGLE_FAN);
            for (int i = 0; i < num_segments; ++i) {
                float angle = 2.0f * 3.1415926f * float(i) / float(num_segments);
                float dx = radius * cosf(angle);
                float dy = radius * sinf(angle);
                glVertex2f(x + dx, y + dy);
            }
            glEnd();
        }

};

int main() {

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "ESC", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Entity player;
    player.positions.push_back({0.0f, 0.0f});
    player.velocities.push_back({0.01f, 0.0f});

    MovementSystem movementSystem;
    CircleRenderingSystem circle_rendering_system;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        movementSystem.update(player);

        glClear(GL_COLOR_BUFFER_BIT);
        for (size_t i = 0; i < player.positions.size(); ++i) {
            circle_rendering_system.drawCircle(player.positions[i].x, player.positions[i].y, 0.3f);
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
