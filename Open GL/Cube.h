#pragma once
#include "Vector.h"

class Cube
{
public:
	Cube() { 
		Size = Vector2(1.0f, 1.0f);
		Mass = 1.0f; };
	Vector2 Position;
	Vector2 Size;//Size(1.0f, 1.0f);
	Vector2 Velocity;
	Vector2 Acceleration;
	Vector4 Color;
	float Mass;
};

