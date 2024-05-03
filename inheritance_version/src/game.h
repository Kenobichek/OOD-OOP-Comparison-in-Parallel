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
			for (auto& obj : objects) {
				thread_pool->AddTask([&]() {
					if (!obj) return;
					obj->Update();
				});
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