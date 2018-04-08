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

	void SetRotation(float deg) // ȸ��
	{
		float radian = Deg2Rad(deg);

		_11 = cosf(deg);
		_12 = -sinf(deg);
		_21 = sinf(deg);
		_22 = cosf(deg);
	}

	void SetIdentity()	// �׵�
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
	};

	void SetScale(float scale) {	// ������
		_11 = scale;
		_12 = 0.0f;
		_21 = 0.0f;
		_22 = scale;
	}

	void SetShear(float shear) {	// �б�
		_11 = 1.0f;
		_12 = shear;
		_21 = shear;
		_22 = 1.0f;
	}
	void SetReflect(float reflect) {	// �ݻ�
		_11 = -1.0f;
		_12 = 0.0f;
		_21 = 0.0f;
		_22 = -1.0f;
	}

	Matrix2 operator *(const Matrix2 Other) const;
};


struct Matrix3
{
public:
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3() {
		_11 = 0.0;
		_12 = 0.0;
		_13 = 0.0;
		_21 = 0.0;
		_22 = 0.0;
		_23 = 0.0;
		_31 = 0.0;
		_32 = 0.0;
		_33 = 0.0;
	};


	void SetIdentity()
	{
		_11 = 1.0;
		_12 = 0.0;
		_13 = 0.0;
		_21 = 0.0;
		_22 = 1.0;
		_23 = 0.0;
		_31 = 0.0;
		_32 = 0.0;
		_33 = 1.0;
	};

	void SetTranform(float trX, float trY) {
		_11 = 1.0;
		_12 = 0.0;
		_13 = trX;
		_21 = 0.0;
		_22 = 1.0;
		_23 = trY;
		_31 = 0.0;
		_32 = 0.0;
		_33 = 1.0;
	}

};
