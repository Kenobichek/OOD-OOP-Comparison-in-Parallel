#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Object {
	public:
		enum EShape {
			Circle,
			Square
		};

		Object() {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis2(0.01f, 0.1f);
			std::uniform_real_distribution<float> dis(0.0f, 2 * M_PI);

			float angle = dis(gen);
			dx = cos(angle);
			dy = sin(angle);

			speed = dis2(gen);

			x = 0;
			y = 0;

			color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

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

			shape = EShape::Circle;
		}

		Object(float posX, float posY, float spdX, float spdY, EShape shape)
			: x(posX), y(posY), dx(spdX), dy(spdY), color(1.0f, 1.0f, 1.0f, 1.0f) {}

		virtual void Update() = 0;
		virtual void Draw() const = 0;
			
		void SetColor(float r, float g, float b, float a = 1.0f) {
			color = ImVec4(r, g, b, a);
		}

		ImVec4 GetColor() const {
			return color;
		}
		
	protected:
		float x, y;
		float dx, dy;
		float speed;
		ImVec4 color;
		EShape shape;
};

class Circle : public Object {
	public:
		Circle() : Object(), radius(0.01f) {}

		Circle(float posX, float posY, float spdX, float spdY, float rad, EShape shape = EShape::Circle) : Object(posX, posY, spdX, spdY, shape), radius(rad) {}

		void Update() override {
			{
				ZoneScopedN("x y");
				x += dx * speed;
				y += dy * speed;
				if (x - radius / 2 < -1  || x + radius / 2  > 1) {
					x = x < 0 ? radius / 2  - 1 : 1 - radius / 2 ;
					dx *= -1;
				}
				if (y - radius / 2  < -1 || y + radius / 2  > 1) {
					y = y < 0 ? radius / 2 - 1 : 1 - radius / 2 ;
					dy *= -1;
				}
			}
		}

		void Draw() const override {
			const int num_segments = 30;

			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i < num_segments; ++i) {
				float angle = 2.0f * M_PI * float(i) / float(num_segments);
				float dx = radius / 2  * cosf(angle);
				float dy = radius / 2  * sinf(angle);
				glVertex2f(x + dx, y + dy);
			}
			glEnd();
		}

	private:
		float radius;
};

class Square : public Object {
	public:
		Square() : Object(), side_length(0.01f) {}

		Square(float posX, float posY, float spdX, float spdY, float side, EShape shape = EShape::Square) 
			: Object(posX, posY, spdX, spdY, shape), side_length(side) {}

		void Update() override {
			x += dx * speed;
			y += dy * speed;

			if (x - side_length / 2 < -1  || x + side_length / 2 > 1) {
				x = x < 0 ? side_length/2 - 1 : 1 - side_length/2;
				dx *= -1;
			}
			if (y - side_length / 2 < -1 || y + side_length / 2 > 1) {
				y = y < 0 ? side_length / 2 - 1 : 1 - side_length / 2;
				dy *= -1;
			}
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
