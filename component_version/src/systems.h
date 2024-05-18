#pragma once
#include <memory>
#include <cmath> 
#include <tracy/Tracy.hpp>
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
		ZoneScopedN("Add task");
		size_t num_threads = thread_pool->GetThreadCount();
		int chunk_size = entity_manager->GetObjectCount() / num_threads;
		int remaining_size = entity_manager->GetObjectCount() % num_threads;
		
		size_t start_i = 0;
		
		for (size_t i = 0; i < num_threads; ++i) {
			int end_i = start_i == 0 ? chunk_size + remaining_size : chunk_size;
			end_i += start_i;

			auto& entities = entity_manager->GetEntities();
			auto start_it = entities.begin() + start_i;
			auto end_it = entities.begin() + end_i;

			thread_pool->AddTask([=]() {
				for (auto it = start_it; it != end_it; ++it) {
					auto& entity = *it;
					if (!entity) {
						return;
					}

					auto position = entity->GetComponent<Position>();
					auto dimension = entity->GetComponent<Dimension>();
					auto velocity = entity->GetComponent<Velocity>();

					if (position && velocity && dimension) {
						position->x += velocity->dx * velocity->speed;
						position->y += velocity->dy * velocity->speed;
					}

					if (IsOutOfBounds(entity)) {
						HandleCollisionWithBounds(entity);
					}

					for (auto& other_entity : entity_manager->GetEntities()) {
						if (entity == other_entity || !other_entity) continue;
						
						float toi = TimeOfImpact(entity, other_entity);
    
						if (toi >= 0.0f && toi <= velocity->speed) {
							HandleCollision(entity, other_entity);
						}
					}
				}
			});

			start_i = end_i;
		}
	}

	private:
		std::shared_ptr<EntityManager> entity_manager;
		std::shared_ptr<ThreadPool> thread_pool;

		bool CheckCollision(std::unique_ptr<Entity> a_entity, std::unique_ptr<Entity> b_entity);
		bool IsOutOfBounds(std::unique_ptr<Entity>& entity);

		void HandleCollisionWithBounds(std::unique_ptr<Entity>& entity);
		void HandleCollision(std::unique_ptr<Entity>& a_entity, std::unique_ptr<Entity>& b_entity);

		float TimeOfImpact(std::unique_ptr<Entity>& a_entity, std::unique_ptr<Entity>& b_entity);
};

class CircleRenderingSystem {
	public:
		CircleRenderingSystem(std::shared_ptr<EntityManager> manager) : entity_manager(manager) {}
		
		void Draw() {
			const int num_segments = 30;

			for (auto& entity : entity_manager->GetEntities()) {
				if (!entity) return;

				auto shape = entity->GetComponent<Shape>();
				if (!shape) {
					return;
				}
				if (shape->shape != Shape::EShape::Circle) {
					continue;
				}

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
					float dx = dimension->width / 2 * cosf(angle);
					float dy = dimension->height / 2 * sinf(angle);
					glVertex2f(position->x + dx, position->y + dy);
				}
				glEnd();
			}
		}

	private:
		std::shared_ptr<EntityManager> entity_manager;
};

class SquareRenderingSystem {
	public:
		SquareRenderingSystem(std::shared_ptr<EntityManager> manager) : entity_manager(manager) {}

		void Draw() {
			for (auto& entity : entity_manager->GetEntities()) {
				if (!entity) return;

				auto shape = entity->GetComponent<Shape>();
				if (!shape) {
					return;
				}
				if (shape->shape != Shape::EShape::Square) {
					continue;
				}

				auto dimension = entity->GetComponent<Dimension>();
				
				if (!dimension) {
					return;
				}
				auto position = entity->GetComponent<Position>();
				if (!position) {
					return;
				}

				glBegin(GL_QUADS);
				glVertex2f(position->x - dimension->width / 2, position->y - dimension->height / 2);
				glVertex2f(position->x + dimension->width / 2, position->y - dimension->height / 2);
				glVertex2f(position->x + dimension->width / 2, position->y + dimension->height / 2);
				glVertex2f(position->x - dimension->width / 2, position->y + dimension->height / 2);
				glEnd();
			}
		}

	private:
		std::shared_ptr<EntityManager> entity_manager;
};
