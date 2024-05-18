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
		virtual ~Object() {}

	protected:
		double x, y;
		double dx, dy;
		double speed;
};


class Drawable : public virtual Object {
	public:
		virtual void Draw() const = 0;
};


class Movable : public virtual Object {
	public:
		virtual void Move() {
			x += dx * speed;
			y += dy * speed;
		};
};


class Collidable : public virtual Object {
	public:
		virtual void HandleCollisionWithBounds() = 0;
};


class Shape : public Drawable, public Movable, public Collidable {
	public:
		Shape () {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> angle_dist(0.0f, 2 * M_PI);
			std::uniform_real_distribution<float> speed_dist(0.01f, 0.1f);
			std::uniform_real_distribution<float> position_dist(-1.0f, 1.0f);

			x = position_dist(gen);
			y = position_dist(gen);

			float angle = angle_dist(gen);
			dx = std::cos(angle);
			dy = std::sin(angle);

			speed = speed_dist(gen);
		};

		virtual void Draw() const = 0;
		virtual void HandleCollisionWithBounds() = 0;
};


class Circle : public Shape {

	public:
		Circle() : radius(0.01f) {}

		virtual void Draw() const override {
			const int num_segments = 30;

			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i < num_segments; ++i) {
				float angle = 2.0f * M_PI * float(i) / float(num_segments);
				float dx = radius / 2 * cosf(angle);
				float dy = radius / 2 * sinf(angle);
				glVertex2f(x + dx, y + dy);
			}
			glEnd();
		}


		virtual void HandleCollisionWithBounds() override {
			if (x - radius / 2 < -1) {
				x = -1 + radius / 2;
				dx *= -1;
			} else if (x + radius / 2 > 1) {
				x = 1 - radius / 2;
				dx *= -1;
			}

			if (y - radius / 2 < -1) {
				y = -1 + radius / 2;
				dy *= -1;
			} else if (y + radius / 2 > 1) {
				y = 1 - radius / 2;
				dy *= -1;
			}
		}

	private:
		double radius;
};


class Square : public Shape {

	public:
		Square() : side_length(0.01f) {}

		virtual void Draw() const override {
			glBegin(GL_QUADS);
			glVertex2f(x - side_length / 2, y - side_length / 2);
			glVertex2f(x + side_length / 2, y - side_length / 2);
			glVertex2f(x + side_length / 2, y + side_length / 2);
			glVertex2f(x - side_length / 2, y + side_length / 2);
			glEnd();
		}

		virtual void HandleCollisionWithBounds() override {
			if (x - side_length / 2 < -1) {
				x = -1 + side_length / 2;
				dx *= -1;
			} else if (x + side_length / 2 > 1) {
				x = 1 - side_length / 2;
				dx *= -1;
			}

			if (y - side_length / 2 < -1) {
				y = -1 + side_length / 2;
				dy *= -1;
			} else if (y + side_length / 2 > 1) {
				y = 1 - side_length / 2;
				dy *= -1;
			}
		}

	private:
		float side_length;
};
