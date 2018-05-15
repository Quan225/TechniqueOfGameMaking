#pragma once

#include "stdafx.h"
#include "Vertex.h"

class Mesh
{
public:

	Vertex* Vertices; // 버텍스
	int* Indices;	// 인덱스
	int ISize; // 인덱스 개수
	int VSize; // 버텍스 개수

public:
	Mesh() : Vertices(NULL), Indices(NULL), ISize(0), VSize(0) { }
	~Mesh ();
	void SetVertetices (Vertex* NewVertices, size_t NewSize);
	void SetIndices (int* NewIndices, size_t NewSize);

};