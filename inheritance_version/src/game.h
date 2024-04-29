#pragma once
#include <vector>
#include <memory>
#include "objects.h"

using ObjectVector = std::vector<std::unique_ptr<Object>>;

class Game {
	public:
		void AddObject(std::unique_ptr<Object> obj) {
			objects.push_back(std::move(obj));
		}

		void Update() {
			for (auto& obj : objects) {
				obj->Update();
			}
		}

		void Draw() const {
			for (const auto& obj : objects) {
				obj->Draw();
			}
		}
		
	private:
		ObjectVector objects;
};