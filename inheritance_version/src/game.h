#pragma once
#include <vector>
#include <memory>
#include "objects.h"
#include "thread_pool.h"

constexpr int MAX_OBJECTS = 1000;

using ObjectVector = std::vector<std::unique_ptr<Object>>;

class Game {
	public:
		Game(std::shared_ptr<ThreadPool> thread_pool) 
			: thread_pool(thread_pool) {}

		void AddObject(std::unique_ptr<Object> obj) {
			objects.push_back(std::move(obj));
		}

		void RemoveLastObject() {
			if (!objects.empty()) {
				objects.pop_back();
			}
		}

		int GetObjectCount() const {
			return objects.size();
		}

		void Update() {
			size_t num_threads = thread_pool->GetThreadCount();
			int chunk_size = GetObjectCount() / num_threads;
			int remaining_size = GetObjectCount() % num_threads;

			size_t start_i = 0;

			for (size_t i = 0; i < num_threads; ++i) {
				int end_i = start_i == 0 ? chunk_size + remaining_size : chunk_size;
				end_i += start_i;
			
				thread_pool->AddTask([=]() {
					for (size_t i = start_i; i < end_i; ++i) {
						objects[i]->Update();
					}
				});

				start_i = end_i;
			}
		}

		void Draw() const {
			for (const auto& obj : objects) {
				obj->Draw();
			}
		}
		
	private:
		ObjectVector objects;
		std::shared_ptr<ThreadPool> thread_pool;
};