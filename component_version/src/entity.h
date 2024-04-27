#pragma once
#include <vector>
#include <memory>
#include "components.h"

using Components = std::vector<std::shared_ptr<Component>>;

class Entity {
	public:
		template<typename T>
		Entity& AddComponent() {
			components.push_back(std::make_shared<T>());
			return *this;
		}

		template<typename T>
		Entity& AddComponent(std::shared_ptr<T>&& component) {
			components.push_back(component);
			return *this;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent() {
			for (const auto& component : components) {
				if (auto comp = std::dynamic_pointer_cast<T>(component)) {
					return comp;
				}
			}
			return nullptr;
		}

	private:
		Components components;
};