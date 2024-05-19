#pragma once
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct Component
{
	virtual ~Component() {}
};

struct Position : public Component {
	Position() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
		
		x = dis(gen);
		y = dis(gen);
	}

	Position(float ax, float ay) : x(ax), y(ay) {}

	float x;
	float y;
};

struct Velocity : public Component {
	Velocity() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis2(0.01f, 0.1f);
		std::uniform_real_distribution<float> dis(0.0f, 2 * M_PI);

		float angle = dis(gen);
		dx = cos(angle);
		dy = sin(angle);

		speed = dis2(gen);
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

struct Shape : public Component {
	enum EShape {
		Circle,
		Square
	};

	Shape() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> distribution(0, 1);

		int randomShape = distribution(gen);

		switch (randomShape) {
			case 0:
				shape = EShape::Square;
				break;
			case 1:
				shape = EShape::Circle;
				break;
			default:
				shape = EShape::Square;
				break;
		}

	}

	EShape shape;
};
