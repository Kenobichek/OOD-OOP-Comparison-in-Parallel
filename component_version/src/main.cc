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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Component Version", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	auto entity_manager = std::make_shared<EntityManager>();
	MovementSystem movement_system(entity_manager);
	RenderingSystem circle_rendering_system(entity_manager);

	int slider_object_count = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		movement_system.Update();

		glClear(GL_COLOR_BUFFER_BIT);
		circle_rendering_system.Draw();

		ImGui::SetNextWindowPos(ImVec2(20, 10));
		ImGui::SetNextWindowSize(ImVec2(300, 80));

		ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("Num Objects: %d", slider_object_count);
        ImGui::SliderInt("##Num Objects", &slider_object_count, 0, MAX_OBJECTS);
		ImGui::End();

		if (entity_manager->GetObjectCount() > slider_object_count) {
			entity_manager->RemoveLastEntity();
		}
		else if (entity_manager->GetObjectCount() < slider_object_count) {
			entity_manager->CreateEntity()
				.AddComponent(std::make_shared<Position>())
				.AddComponent(std::make_shared<Velocity>())
				.AddComponent(std::make_shared<Dimension>());
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
