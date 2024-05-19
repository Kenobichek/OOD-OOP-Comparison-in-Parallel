#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Shape;

class Object {
	public:
		virtual ~Object() {}
		
		double GetX() const { return x; }
		double GetY() const { return y; }
		double GetDx() const { return dx; }
		double GetDy() const { return dy; }
		double GetSpeed() const { return speed; }

        void SetX(double newX) { x = newX; }
        void SetY(double newY) { y = newY; }
        void SetDx(double newDx) { dx = newDx; }
        void SetDy(double newDy) { dy = newDy; }
        void SetSpeed(double newSpeed) { speed = newSpeed; }

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
        virtual float TimeOfImpact(const std::unique_ptr<Shape>& shape) const = 0;
		virtual void HandleCollision(const std::unique_ptr<Shape>& shape) = 0;
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

        virtual float TimeOfImpact(const std::unique_ptr<Shape>& shape) const override {
			float dx = this->x - shape->GetX();
			float dy = this->y - shape->GetY();
			float dvx = this->dx - shape->GetDx();
			float dvy = this->dy - shape->GetDy();
			float r = this->GetSize() + shape->GetSize();
			
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
		};

		virtual void HandleCollision(const std::unique_ptr<Shape>& shape) {
			double a_mass = shape->GetSize() * shape->GetSize();
			double b_mass = this->GetSize() * this->GetSize();

			float dx = this->x - shape->GetX();
			float dy = this->y - shape->GetY();

			float distance = std::sqrt(dx * dx + dy * dy);
			float overlap = 0.5f * (distance - shape->GetSize() / 2 - this->GetSize() / 2);

			// Displace a_entity
			shape->SetX(shape->GetX() - overlap * (shape->GetX() - this->GetX()) / distance);
			shape->SetY(shape->GetY() - overlap * (shape->GetY() - this->GetY()) / distance);

			// Displace b_entity
			this->x += overlap * (shape->GetX()  - this->x) / distance;
			this->y += overlap * (shape->GetY() - this->y) / distance;

			float collisionAngle = std::atan2(dy, dx);
			double magnitudeA = std::sqrt(shape->GetDx() * shape->GetDx() + shape->GetDy() * shape->GetDy());
			float magnitudeB = std::sqrt(this->dx * this->dx + this->dy * this->dy);

			double directionA = std::atan2(shape->GetDy(), shape->GetDx());
			float directionB = std::atan2(this->dy, this->dx);

			float newVelXA = (magnitudeA * std::cos(directionA - collisionAngle) * (a_mass - b_mass) + 2 * b_mass * magnitudeB * std::cos(directionB - collisionAngle)) / (a_mass + b_mass) * std::cos(collisionAngle) + magnitudeA * std::sin(directionA - collisionAngle) * std::cos(collisionAngle + M_PI / 2);
			float newVelYA = (magnitudeA * std::cos(directionA - collisionAngle) * (a_mass - b_mass) + 2 * b_mass * magnitudeB * std::cos(directionB - collisionAngle)) / (a_mass + b_mass) * std::sin(collisionAngle) + magnitudeA * std::sin(directionA - collisionAngle) * std::sin(collisionAngle + M_PI / 2);
			float newVelXB = (magnitudeB * std::cos(directionB - collisionAngle) * (b_mass - a_mass) + 2 * a_mass * magnitudeA * std::cos(directionA - collisionAngle)) / (a_mass + b_mass) * std::cos(collisionAngle) + magnitudeB * std::sin(directionB - collisionAngle) * std::cos(collisionAngle + M_PI / 2);
			float newVelYB = (magnitudeB * std::cos(directionB - collisionAngle) * (b_mass - a_mass) + 2 * a_mass * magnitudeA * std::cos(directionA - collisionAngle)) / (a_mass + b_mass) * std::sin(collisionAngle) + magnitudeB * std::sin(directionB - collisionAngle) * std::sin(collisionAngle + M_PI / 2);

			shape->SetDx(newVelXA);
			shape->SetDy(newVelYA);
			this->dx = newVelXB;
			this->dy = newVelYB;
		}

		virtual void Draw() const = 0;
		virtual void HandleCollisionWithBounds() = 0;
		virtual float GetSize() const = 0;
};


class Circle : public Shape {

	public:
		Circle() : radius(0.1f) {}

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

		virtual float GetSize() const override {
			return radius;
		}

	private:
		double radius;
};


class Square : public Shape {

	public:
		Square() : side_length(0.1f) {}

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

		virtual float GetSize() const override {
			return side_length;
		}

	private:
		float side_length;
};
