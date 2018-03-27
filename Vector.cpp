#include "stdafx.h"
#include "Vector.h"

float Vector2::Dist(const Vector2 &V1, const Vector2 &V2)
{
	return sqrtf(Vector2::DistSquared(V1, V2));
}

float Vector2::DistSquared(const Vector2 &V1, const Vector2 &V2)
{
	return (V2.X - V1.X) * (V2.X - V1.X) + (V2.Y - V1.Y) * (V2.Y - V1.Y);
}

Vector2 Vector2::operator*(Matrix2 Mat) const
{
	Vector2 result;
	result.X = X * Mat._11 + Y * Mat._12;
	result.Y = X * Mat._21 + Y * Mat._22;
	return result;
}

