#pragma once
#include <random>

struct Component
{
	virtual ~Component() {}
};

struct Position : public Component {
	Position(float ax = 0.0f, float ay = 0.0f) : x(ax), y(ay) {}

	float x;
	float y;
};

struct Velocity : public Component {
	Velocity() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis2(1.0f, 5.0f);
		std::uniform_real_distribution<float> dis(0.0f, 2 * M_PI);

		float angle = dis(gen);
		dx = cos(angle);
		dy = sin(angle);

		speed = 0.1;
	}

	Velocity(float adx, float ady, float aspeed = 10.0f) : dx(adx), dy(ady), speed(aspeed) {}
	
	float dx;
	float dy;
	float speed;
};

struct Dimension : public Component {
	Dimension(float h = 0.01f, float w = 0.01f) : height(h), width(w) {}
	
	float height;
	float width;
};

struct Color : public Component {
	Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : color(r, g, b, a) {}

	ImVec4 color;
};
