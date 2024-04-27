#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "entity_manager.h"
#include "systems.h"

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

	auto entity_manager = std::make_shared<EntityManager>();
	auto ent = entity_manager->CreateEntity()
		.AddComponent(std::make_shared<Position>(0, 0))
		.AddComponent(std::make_shared<Velocity>(0.001f, 0));

	MovementSystem movement_system(entity_manager);
	RenderingSystem circle_rendering_system(entity_manager);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		movement_system.Update();

		glClear(GL_COLOR_BUFFER_BIT);
		circle_rendering_system.Draw();

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
