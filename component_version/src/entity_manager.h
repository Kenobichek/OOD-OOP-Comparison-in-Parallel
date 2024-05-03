#pragma once
#include <memory>
#include <algorithm>
#include "entity.h"

constexpr int MAX_OBJECTS = 1000;

using EntityVector = std::vector<std::unique_ptr<Entity>>;

class EntityManager {
	public:
		Entity& CreateEntity() {
			entities.push_back(std::make_unique<Entity>());
			return *entities.back();
		}

		void DestroyEntity(Entity& entity) {
			entities.erase(std::remove_if(entities.begin(), entities.end(),
				[&](const std::unique_ptr<Entity>& e) { return e.get() == &entity; }), entities.end());
		}
		
		EntityVector& GetEntities() {
			return entities;
		}

		int GetObjectCount() const {
			return entities.size();
		}

		std::unique_ptr<Entity> RemoveLastEntity() {
			if (!entities.empty()) {
				auto last_entity = std::move(entities.back());
				entities.pop_back();
				return last_entity;
			}
			return nullptr;
		}

	private:
		EntityVector entities;
};