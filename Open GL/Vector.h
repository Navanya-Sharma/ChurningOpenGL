#pragma once
struct Vector2
{
public:
	Vector2() {
		x = 0.0f;
		y = 0.0f;
	};
	Vector2(float a, float b) {
		x = a;
		y = b;
	};
	float x;
	float y;

	Vector2 operator+(const Vector2 other) {
		Vector2 result;
		result.x = x + other.x;
		result.y = y + other.y;
		return result;
	}
	Vector2 operator+=(const Vector2 other) {
		 x += other.x;
		 y += other.y;
		return *this;
	}
	Vector2 operator*(const float f) {
		Vector2 result;
		result.x = x *f;
		result.y = y *f;
		return result;
	}
};
struct Vector4
{
public:
	Vector4() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	};
	Vector4(float a, float b,float c, float d) {
		x = a;
		y = b;
		z = c;
		w = d;
	};
	float x, y, z, w;

	Vector4 operator+(const Vector4 other) {
		Vector4 result;
		result.x = x + other.x;
		result.y = y + other.y;
		result.w = w + other.w;
		result.z = z + other.z;
		return result;
	}
	Vector4 operator+=(const Vector4 other) {
		 x += other.x;
		 y += other.y;
		 w += other.w;
		 z += other.w;
		return *this;
	}
	Vector4 operator*(const float f) {
		Vector4 result;
		result.x = x *f;
		result.y = y *f;
		result.w = w *f;
		result.z = z *f;
		return result;
	}
};

