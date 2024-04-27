#pragma once

struct Component
{
	virtual ~Component() {}
};

struct Position : public Component {
	Position(float ax, float ay) : x(ax), y(ay) {}

	float x;
	float y;
};

struct Velocity : public Component {
	Velocity(float adx, float ady) : dx(adx), dy(ady) {}
	
	float dx;
	float dy;
};