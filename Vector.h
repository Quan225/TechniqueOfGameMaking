#pragma once

#include "stdafx.h"
#include "Matrix.h"

struct Vector2 {
public:
	float x;
	float y;

	Vector2() : x(0), y(0) {}
	Vector2(float X, float Y) : x(X), y(Y) {}

	static float Distance(const Vector2 &startVec, const Vector2 &endVec);
	static float DistanceSquared(const Vector2 &startVec, const Vector2 &endVec);
	
};


struct Vector3 {
public:
	float x;
	float y;
	float z;
};