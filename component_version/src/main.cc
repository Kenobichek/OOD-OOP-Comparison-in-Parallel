#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 
#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "entity_manager.h"
#include "systems.h"
#include "thread_pool.h"

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

	auto thread_pool = std::make_shared<ThreadPool>(4);
	auto entity_manager = std::make_shared<EntityManager>();
	
	MovementSystem movement_system(entity_manager, thread_pool);
	CircleRenderingSystem circle_rendering_system(entity_manager);
	SquareRenderingSystem square_rendering_system(entity_manager);

	int slider_object_count = 0;
	int object_count = 0;

	ZoneScoped;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(20, 10));
		ImGui::SetNextWindowSize(ImVec2(300, 80));
		ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("Num Objects: %d", slider_object_count);
		ImGui::SliderInt("##Num Objects", &slider_object_count, 0, MAX_OBJECTS);
		ImGui::End();

		if (object_count > slider_object_count) {
			int dif = object_count - slider_object_count;
			for (auto i = 0; i < dif; i++) {
				entity_manager->RemoveLastEntity();
			}
			object_count = slider_object_count;
		}
		else if (object_count < slider_object_count) {
			int dif = slider_object_count - object_count;
			for (auto i = 0; i < dif; i++) {
				entity_manager->CreateEntity()
					.AddComponent(std::make_shared<Position>())
					.AddComponent(std::make_shared<Velocity>())
					.AddComponent(std::make_shared<Dimension>(0.01, 0.01))
					.AddComponent(std::make_shared<Shape>());
			}
			object_count = slider_object_count;
		}
		{
			ZoneScopedN("UpdateSystems");
			movement_system.Update();
			thread_pool->WaitForAllThreads();
		}
		{
			ZoneScopedN("RenderFrame");
			glClear(GL_COLOR_BUFFER_BIT);
			circle_rendering_system.Draw();
			square_rendering_system.Draw();
		}

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);

		FrameMark;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
