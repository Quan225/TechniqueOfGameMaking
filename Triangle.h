#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vector3 position;
	ULONG color;
public:
	Vertex() {}

	Vertex(Vector3 pos) {
		position = pos;
		color = 0;
	}

	Vertex(Vector3 pos, ULONG _color) {
		position = pos;
		color = _color;
	}


	void setPosition(Vector3 pos) { position = pos; }
	Vector3 getPosition() { return position; }
};

struct Triangle
{
public:
	Vertex vertices[3];
	Vertex localVertices[3];

public:
	Triangle() {}

	Triangle(const Vertex& up, const Vertex& left, const Vertex& right) {
		vertices[0] = up;
		vertices[1] = left;
		vertices[2] = right;

		initLocalVertex();

	}

	Triangle(const Vector3& up, const Vector3& left, const Vector3& right) {

		vertices[0].position = up;
		vertices[1].position = left;
		vertices[2].position = right;

		initLocalVertex();

	}

	void initLocalVertex() {

		Vector3 temp1 = vertices[0].position;
		Vector3 temp2 = (vertices[1].position + temp1)* 0.5f; //중점
		Vector3 Pt = temp2 * (1.0f - 0.33) + vertices[2].position * 0.33; // 삼각형의 무게중심

		localVertices[0] = vertices[0].position - Pt;
		localVertices[1] = vertices[1].position - Pt;
		localVertices[2] = vertices[2].position - Pt;

		localVertices[0].position.Z = 1;
		localVertices[1].position.Z = 1;
		localVertices[2].position.Z = 1;
	}

	bool IsInTriangle() { return false; }

};

