#pragma once
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Object {
	public:
		Object(float posX, float posY, float spdX, float spdY) : x(posX), y(posY), dx(spdX), dy(spdY) {}

		virtual void Update() = 0;
		virtual void Draw() const = 0;

	protected:
		float x, y;
		float dx, dy;
};

class Circle : public Object {
	public:
		Circle(float posX, float posY, float spdX, float spdY, float rad) : Object(posX, posY, spdX, spdY), radius(rad) {}

		void Update() override {
			x += dx;
			y += dy;
		}

		void Draw() const override {
			const int num_segments = 30;
			const float radius = 0.3;

			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i < num_segments; ++i) {
				float angle = 2.0f * M_PI * float(i) / float(num_segments);
				float dx = radius * cosf(angle);
				float dy = radius * sinf(angle);
				glVertex2f(x + dx, y + dy);
			}
			glEnd();
		}

	private:
		int radius;
};

class Square : public Object {
	public:
		Square(float posX, float posY, float spdX, float spdY, float side) : Object(posX, posY, spdX, spdY), side_length(side) {}

		void Update() override {
			x += dx;
			y += dy;
		}

		void Draw() const override {
			glBegin(GL_QUADS);
			glVertex2f(x - side_length / 2, y - side_length / 2);
			glVertex2f(x + side_length / 2, y - side_length / 2);
			glVertex2f(x + side_length / 2, y + side_length / 2);
			glVertex2f(x - side_length / 2, y + side_length / 2);
			glEnd();

		}

	private:
		float side_length;
};
