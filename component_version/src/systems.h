#pragma once
#include <memory>
#include <cmath> 
#include "entity_manager.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class MovementSystem {
	public:
		MovementSystem(std::shared_ptr<EntityManager> manager) : entity_manager(manager) {}

		void Update() {
			for (auto& entity : entity_manager->GetEntities()) {
				auto position = entity->GetComponent<Position>();
				auto velocity = entity->GetComponent<Velocity>();

				if (position && velocity) {
					position->x += velocity->dx;
					position->y += velocity->dy;
				}
			}
		}
		
	private:
		std::shared_ptr<EntityManager> entity_manager;
};

class RenderingSystem {
	public:
		RenderingSystem(std::shared_ptr<EntityManager> manager) : entity_manager(manager) {}
		
		void Draw() {
			const int num_segments = 30;
			const float radius = 0.3;
			for (auto& entity : entity_manager->GetEntities()) {
				auto position = entity->GetComponent<Position>();

				glBegin(GL_TRIANGLE_FAN);
				for (int i = 0; i < num_segments; ++i) {
					float angle = 2.0f * M_PI * float(i) / float(num_segments);
					float dx = radius * cosf(angle);
					float dy = radius * sinf(angle);
					glVertex2f(position->x + dx, position->y + dy);
				}
				glEnd();
			}
		}

	private:
		std::shared_ptr<EntityManager> entity_manager;

};
