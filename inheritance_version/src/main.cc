#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "game.h"

int main() {

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 800, "Inheritance Version", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

    Game game;

	int sliderObjectCount = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		game.Update();
		glClear(GL_COLOR_BUFFER_BIT);

		game.Draw();

		ImGui::SetNextWindowPos(ImVec2(20, 10));
		ImGui::SetNextWindowSize(ImVec2(300, 80));

		ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("Num Objects: %d", sliderObjectCount);
        ImGui::SliderInt("##Num Objects", &sliderObjectCount, 0, MAX_OBJECTS);
		ImGui::End();

		if (game.GetObjectCount() > sliderObjectCount) {
			game.RemoveLastObject();
		}
		else if (game.GetObjectCount() < sliderObjectCount) {
			game.AddObject(std::make_unique<Circle>());
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