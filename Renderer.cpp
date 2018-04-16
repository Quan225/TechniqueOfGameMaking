/*
삼각형 단위로 물체를 렌더링 할 것.
Vertex , Triangle 두 구조체를 사용해서 '로컬 좌표계' 삼각형을 정의하고,
Modeling Matrix를 통해 삼각형을 이동 / 회전 / 스케일을 조정하는 프로그램을 만들 것!

mvp (model, view, projection)

*/


#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + pt.X + g_nClientWidth * -pt.Y;
	*(dest + offset) = g_CurrentColor;
}

void DrawLine(const Vector3& start, const Vector3& end)
{
	float length = (end - start).Dist();
	float inc = 1.0f / length;

	int maxLength = RoundToInt(length);
	for (int i = 0; i <= maxLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 Pt = start * (1.0f - t) + end * t;
		PutPixel(Pt.ToIntPoint());
	}

}



void Draw2DTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3) 
{
	float xMin, yMin;
	float xMax, yMax;
	xMin = yMin = INFINITY;
	xMax = yMax = -INFINITY;

	if (v1.X < xMin) xMin = v1.X;
	if (v2.X < xMin) xMin = v2.X;
	if (v3.X < xMin) xMin = v3.X;
	if (v1.X > xMax) xMax = v1.X;
	if (v2.X > xMax) xMax = v2.X;
	if (v3.X > xMax) xMax = v3.X;
	if (v1.Y < yMin) yMin = v1.Y;
	if (v2.Y < yMin) yMin = v2.Y;
	if (v3.Y < yMin) yMin = v3.Y;
	if (v1.Y > yMax) yMax = v1.Y;
	if (v2.Y > yMax) yMax = v2.Y;
	if (v3.Y > yMax) yMax = v3.Y;

	Vector2 u = (v2 - v1).ToVector2();
	Vector2 v = (v3 - v1).ToVector2();

	float dotUU = u.Dot(u);
	float dotUV = u.Dot(v);
	float dotVV = v.Dot(v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);


	for (int y = RoundToInt(yMin); y < RoundToInt(yMax); y++)
	{
		for (int x = RoundToInt(xMin); x < RoundToInt(xMax); x++)
		{
			Vector2 w = (Vector3((float)x, (float)y, 0.0f) - v1).ToVector2();
			float dotUW = u.Dot(w);
			float dotVW = v.Dot(w);
			float outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;



			if (outS < 0.0f) continue;
			if (outT < 0.0f) continue;
			if (outS + outT > 1.0f) continue;

			PutPixel(IntPoint(x, y));


		}
	}
}

void Draw2DTriangle(const Triangle& tri, const Matrix3& mat) 
{
	Vector3 v1 = tri.localVertices[0].position * mat;
	Vector3 v2 = tri.localVertices[1].position * mat;
	Vector3 v3 = tri.localVertices[2].position * mat;
	float xMin, yMin;
	float xMax, yMax;
	xMin = yMin = INFINITY;
	xMax = yMax = -INFINITY;

	if (v1.X < xMin) xMin = v1.X;
	if (v2.X < xMin) xMin = v2.X;
	if (v3.X < xMin) xMin = v3.X;
	if (v1.X > xMax) xMax = v1.X;
	if (v2.X > xMax) xMax = v2.X;
	if (v3.X > xMax) xMax = v3.X;
	if (v1.Y < yMin) yMin = v1.Y;
	if (v2.Y < yMin) yMin = v2.Y;
	if (v3.Y < yMin) yMin = v3.Y;
	if (v1.Y > yMax) yMax = v1.Y;
	if (v2.Y > yMax) yMax = v2.Y;
	if (v3.Y > yMax) yMax = v3.Y;

	Vector2 u = (v2 - v1).ToVector2();
	Vector2 v = (v3 - v1).ToVector2();

	float dotUU = u.Dot(u);
	float dotUV = u.Dot(v);
	float dotVV = v.Dot(v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);


	for (int y = RoundToInt(yMin); y < RoundToInt(yMax); y++)
	{
		for (int x = RoundToInt(xMin); x < RoundToInt(xMax); x++)
		{
			Vector2 w = (Vector3((float)x, (float)y, 0.0f) - v1).ToVector2();
			float dotUW = u.Dot(w);
			float dotVW = v.Dot(w);
			float outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;



			if (outS < 0.0f) continue;
			if (outT < 0.0f) continue;
			if (outS + outT > 1.0f) continue;

			PutPixel(IntPoint(x, y));


		}
	}
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	Vector3 Pt1, Pt2, Pt3;

	static float offsetX = 0.0f;
	//static float offsetY = 0.0f;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) offsetX -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) offsetX += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_F3)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_F4)) scale *= 0.99f;

	Matrix3 ModelingMatrix;
	ModelingMatrix.SetIdentity();

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(offsetX, 0.0f);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	ModelingMatrix = ModelingMatrix * TMat * RMat * SMat;


	Pt1.SetPoint(30.0f, 30.0f);
	Pt2.SetPoint(-30.0f, -30.0f);
	Pt3.SetPoint(30.0f, -30.0f);

	SetColor(255, 0, 0);
	Draw2DTriangle(Pt1 * ModelingMatrix, Pt2 * ModelingMatrix, Pt3 * ModelingMatrix);

	SetColor(70, 0, 0);
	Triangle tri(Pt1, Pt2, Pt3);
	Draw2DTriangle(tri, ModelingMatrix);


	// Buffer Swap 
	BufferSwap();
}
