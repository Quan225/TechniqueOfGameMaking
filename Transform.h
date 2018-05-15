#pragma once

#include "Vector.h"

struct Transform2D
{
public:
	Transform2D() {}
	Transform2D(Vector2 InLocation, float InRotation, float InScale) : Location(InLocation), Rotation(InRotation), Scale(InScale) {}

	Matrix3 GetTRSMatrix() 
	{
		Matrix3 TMat, RMat, SMat;
		TMat.SetTranslation(Location.X, Location.Y);
		RMat.SetRotation(Rotation);
		SMat.SetScale(Scale);
		return TMat * RMat * SMat; // 주의, 스케일 -> 회전 -> 이동 순서임
	}

	Matrix3 GetViewMatrix()
	{
		Matrix3 TMat, RMat;
		TMat.SetTranslation(-Location.X, -Location.Y);
		RMat.SetRotation(Rotation);
		RMat.Tranpose();
		return RMat * TMat; // 이동 -> 회전 순서
	}

	Vector2 Location;
	float Rotation;
	float Scale;
};