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
				auto dimension = entity->GetComponent<Dimension>();

				if (position && velocity && dimension) {
					position->x += velocity->dx;
					position->y += velocity->dy;

					const float width = dimension->width;
					const float height = dimension->height;

					if (position->x - width < -1  || position->x + width > 1) {
						position->x = position->x < 0 ? width - 1 : 1 - width;
						velocity->dx *= -1;
					}
					if (position->y - height < -1 || position->y + height > 1) {
						position->y = position->y < 0 ? height - 1 : 1 - height;
						velocity->dy *= -1;
					}
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

			for (auto& entity : entity_manager->GetEntities()) {

				auto dimension = entity->GetComponent<Dimension>();
				
				if (!dimension) {
					continue;
				}

				auto position = entity->GetComponent<Position>();

				glBegin(GL_TRIANGLE_FAN);
				for (int i = 0; i < num_segments; ++i) {
					float angle = 2.0f * M_PI * float(i) / float(num_segments);
					float dx = dimension->width * cosf(angle);
					float dy = dimension->height * sinf(angle);
					glVertex2f(position->x + dx, position->y + dy);
				}
				glEnd();
			}
		}

	private:
		std::shared_ptr<EntityManager> entity_manager;

};
