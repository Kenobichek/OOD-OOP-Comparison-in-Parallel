#include "systems.h"

bool MovementSystem::CheckCollision(std::unique_ptr<Entity> a_entity, std::unique_ptr<Entity> b_entity) {
	auto a_position = a_entity->GetComponent<Position>();
	auto a_velocity = a_entity->GetComponent<Velocity>();
	auto a_dimension = a_entity->GetComponent<Dimension>();

	auto b_position = b_entity->GetComponent<Position>();
	auto b_velocity = b_entity->GetComponent<Velocity>();
	auto b_dimension = b_entity->GetComponent<Dimension>();

	if (!a_position || !a_velocity || !a_dimension || !b_position || !b_velocity || !b_dimension) return false;

	float dx = a_position->x - b_position->x;
	float dy = a_position->y - b_position->y;
	float distance = std::sqrt(dx * dx + dy * dy);
	float radius_sum = (a_dimension->width + b_dimension->width) / 2;

	return distance < radius_sum;
}


float MovementSystem::TimeOfImpact(std::unique_ptr<Entity>& a_entity, std::unique_ptr<Entity>& b_entity) {
	auto a_position = a_entity->GetComponent<Position>();
	auto a_velocity = a_entity->GetComponent<Velocity>();
	auto a_dimension = a_entity->GetComponent<Dimension>();

	auto b_position = b_entity->GetComponent<Position>();
	auto b_velocity = b_entity->GetComponent<Velocity>();
	auto b_dimension = b_entity->GetComponent<Dimension>();

	if (!a_position || !a_velocity || !a_dimension || !b_position || !b_velocity || !b_dimension) return false;

	float dx = b_position->x - a_position->x;
	float dy = b_position->y - a_position->y;
	float dvx = b_velocity->dx - a_velocity->dx;
	float dvy = b_velocity->dy - a_velocity->dy;
	float r = a_dimension->width + b_dimension->width;
	
	float a = dvx * dvx + dvy * dvy;
	float b = 2 * (dx * dvx + dy * dvy);
	float c = dx * dx + dy * dy - r * r;
	
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0f;
	} 
	else {
		float t = (-b - std::sqrt(discriminant)) / (2 * a);
		return t;
	}
}


bool MovementSystem::IsOutOfBounds(std::unique_ptr<Entity>& entity) {
	auto position = entity->GetComponent<Position>();
	auto dimension = entity->GetComponent<Dimension>();

	if (!position || !dimension) return false;

	float left_edge = position->x - dimension->width / 2;
	float right_edge = position->x + dimension->width / 2;
	float top_edge = position->y - dimension->height / 2;
	float bottom_edge = position->y + dimension->height / 2;

	return (left_edge < -1 || right_edge > 1 || top_edge < -1 || bottom_edge > 1);
}


void MovementSystem::HandleCollision(std::unique_ptr<Entity>& a_entity, std::unique_ptr<Entity>& b_entity) {
	auto a_position = a_entity->GetComponent<Position>();
	auto a_velocity = a_entity->GetComponent<Velocity>();
	auto a_dimension = a_entity->GetComponent<Dimension>();

	auto b_position = b_entity->GetComponent<Position>();
	auto b_velocity = b_entity->GetComponent<Velocity>();
	auto b_dimension = b_entity->GetComponent<Dimension>();

	if (!a_position || !a_velocity || !a_dimension || !b_position || !b_velocity || !b_dimension) return;

	float a_mass = a_dimension->width * a_dimension->height;
	float b_mass = b_dimension->width * b_dimension->height;

	float dx = b_position->x - a_position->x;
	float dy = b_position->y - a_position->y;

	float distance = std::sqrt(dx * dx + dy * dy);
	float overlap = 0.5f * (distance - a_dimension->width / 2 - b_dimension->width / 2);

	// Displace a_entity
	// a_position->x -= overlap * (a_position->x - b_position->x) / distance;
	// a_position->y -= overlap * (a_position->y - b_position->y) / distance;

	// // Displace b_entity
	// b_position->x += overlap * (a_position->x - b_position->x) / distance;
	// b_position->y += overlap * (a_position->y - b_position->y) / distance;

	float collisionAngle = std::atan2(dy, dx);
	float magnitudeA = std::sqrt(a_velocity->dx * a_velocity->dx + a_velocity->dy * a_velocity->dy);
	float magnitudeB = std::sqrt(b_velocity->dx * b_velocity->dx + b_velocity->dy * b_velocity->dy);

	float directionA = std::atan2(a_velocity->dy, a_velocity->dx);
	float directionB = std::atan2(b_velocity->dy, b_velocity->dx);

	float newVelXA = (magnitudeA * std::cos(directionA - collisionAngle) * (a_mass - b_mass) + 2 * b_mass * magnitudeB * std::cos(directionB - collisionAngle)) / (a_mass + b_mass) * std::cos(collisionAngle) + magnitudeA * std::sin(directionA - collisionAngle) * std::cos(collisionAngle + M_PI / 2);
	float newVelYA = (magnitudeA * std::cos(directionA - collisionAngle) * (a_mass - b_mass) + 2 * b_mass * magnitudeB * std::cos(directionB - collisionAngle)) / (a_mass + b_mass) * std::sin(collisionAngle) + magnitudeA * std::sin(directionA - collisionAngle) * std::sin(collisionAngle + M_PI / 2);
	float newVelXB = (magnitudeB * std::cos(directionB - collisionAngle) * (b_mass - a_mass) + 2 * a_mass * magnitudeA * std::cos(directionA - collisionAngle)) / (a_mass + b_mass) * std::cos(collisionAngle) + magnitudeB * std::sin(directionB - collisionAngle) * std::cos(collisionAngle + M_PI / 2);
	float newVelYB = (magnitudeB * std::cos(directionB - collisionAngle) * (b_mass - a_mass) + 2 * a_mass * magnitudeA * std::cos(directionA - collisionAngle)) / (a_mass + b_mass) * std::sin(collisionAngle) + magnitudeB * std::sin(directionB - collisionAngle) * std::sin(collisionAngle + M_PI / 2);

	a_velocity->dx = newVelXA;
	a_velocity->dy = newVelYA;
	b_velocity->dx = newVelXB;
	b_velocity->dy = newVelYB;
}

void MovementSystem::HandleCollisionWithBounds(std::unique_ptr<Entity>& entity) {
	auto position = entity->GetComponent<Position>();
	auto velocity = entity->GetComponent<Velocity>();
	auto dimension = entity->GetComponent<Dimension>();

	if (!position || !velocity || !dimension) return;

	if (position->x - (dimension->width / 2) < -1) {
		position->x = -1 + (dimension->width / 2);
		velocity->dx *= -1;
	} else if (position->x + (dimension->width / 2) > 1) {
		position->x = 1 - (dimension->width / 2);
		velocity->dx *= -1;
	}

	if (position->y - (dimension->height / 2) < -1) {
		position->y = -1 + (dimension->height / 2);
		velocity->dy *= -1;
	} else if (position->y + (dimension->height / 2) > 1) {
		position->y = 1 - (dimension->height / 2);
		velocity->dy *= -1;
	}
}