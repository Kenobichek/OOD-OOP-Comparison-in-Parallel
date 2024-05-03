#pragma once
#include <memory>
#include <cmath> 
#include "entity_manager.h"
#include "thread_pool.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class MovementSystem {
	public:
		MovementSystem(std::shared_ptr<EntityManager> manager, std::shared_ptr<ThreadPool> thread_pool) 
		: entity_manager(manager), thread_pool(thread_pool) {}


		void Update() {
			for (auto& entity : entity_manager->GetEntities()) {
				thread_pool->AddTask([&]() {
					if (!entity) return;

					auto position = entity->GetComponent<Position>();
					auto velocity = entity->GetComponent<Velocity>();
					auto dimension = entity->GetComponent<Dimension>();
					auto speed = entity->GetComponent<Dimension>();

					if (position && velocity && dimension) {
						position->x += velocity->dx * velocity->speed;
						position->y += velocity->dy * velocity->speed;

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
				});
			}
		}
		
	private:
		std::shared_ptr<EntityManager> entity_manager;
		std::shared_ptr<ThreadPool> thread_pool;
};

class RenderingSystem {
	public:
		RenderingSystem(std::shared_ptr<EntityManager> manager) : entity_manager(manager) {}
		
		void Draw() {
			const int num_segments = 30;

			for (auto& entity : entity_manager->GetEntities()) {
				auto dimension = entity->GetComponent<Dimension>();
				
				if (!dimension) {
					return;
				}
				auto position = entity->GetComponent<Position>();
				if (!position) {
					return;
				}

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
