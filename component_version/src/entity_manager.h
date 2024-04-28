#pragma once
#include <memory>
#include <algorithm>
#include "entity.h"

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


	private:
		EntityVector entities;
};