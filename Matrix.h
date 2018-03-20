#pragma once
#include "stdafx.h"
#include <stdio.h>

struct Matrix2
{
public:
	float _11, _12;
	float _21, _22;
	Matrix2() {};
	Matrix2(float m11, float m12, float m21, float m22)
	{
		_11 = m11;
		_12 = m12;
		_21 = m21;
		_22 = m22;
	};

	void SetScale(float a, float b)
	{
		SetIdentity();
		_11 = a;
		_22 = b;
	}

	void SetRotation(float deg)
	{
		float radian = Deg2Rad(deg);

		_11 = cosf(deg);
		_12 = -sinf(deg);
		_21 = sinf(deg);
		_22 = cosf(deg);
	}

	void SetIdentity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
	};

	Matrix2 operator *(const Matrix2 Other) const;
};