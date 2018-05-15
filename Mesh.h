#pragma once

#include "stdafx.h"
#include "Vertex.h"

class Mesh
{
public:

	Vertex* Vertices; // ���ؽ�
	int* Indices;	// �ε���
	int ISize; // �ε��� ����
	int VSize; // ���ؽ� ����

public:
	Mesh() : Vertices(NULL), Indices(NULL), ISize(0), VSize(0) { }
	~Mesh ();
	void SetVertetices (Vertex* NewVertices, size_t NewSize);
	void SetIndices (int* NewIndices, size_t NewSize);

};