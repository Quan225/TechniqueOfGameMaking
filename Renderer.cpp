
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Vector.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);
void DrawCircle(Vector2, int r);
void DrawRectangle(Vector2 leftTop, Vector2 rightBottom);
Vector2 SetScale(Vector2 dotPos, int hor, int ver);
void DrawLine(Vector3 start, Vector3 dest);


bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

float trX = 0.0f;
float trY = 0.0f;
float rotation = 0.0f;
float scale = 0.0f;

float l_temp_0 = 10.0f;
float l_temp_1 = 10.0f;
float l_temp_2 = -50.0f;
float l_temp_3 = -30.0f;

Vector3 Pt1, Pt2;
Vector2 Pt3, Pt4;

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	PutPixel(0, 0);
	PutPixel(0, 1);
	PutPixel(1, 0);
	PutPixel(1, 1);


	Matrix2 m_temp_1;
	Matrix3 m_temp_2;

	m_temp_1.SetIdentity();
	m_temp_2.SetIdentity();

	Pt3.X = l_temp_0;
	Pt3.Y = l_temp_1;
	Pt4.X = l_temp_2;
	Pt4.Y = l_temp_3;

	//m_temp_1.SetScale(3);
	if (GetAsyncKeyState(VK_UP)) {
		rotation += 0.01f;
		m_temp_1.SetRotation(rotation);
		Pt3 = Pt3 * m_temp_1;
		Pt4 = Pt4 * m_temp_1;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		scale += 0.01f;
		m_temp_1.SetScale(scale);
		Pt3 = Pt3 * m_temp_1;
	//	Pt4 = Pt4 * m_temp_1;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		scale -= 0.01f;
		m_temp_1.SetScale(scale);
		Pt3 = Pt3 * m_temp_1;
	//	Pt4 = Pt4 * m_temp_1;
	}
	

	Pt1.SetPoint(Pt3.X, Pt3.Y);
	Pt2.SetPoint(Pt4.X, Pt4.Y);


	if (GetAsyncKeyState(VK_DOWN)) {
		trX += 0.001f;
		trY += 0.001f;
		m_temp_2.SetTranform(trX, trY);

		Pt1 = (Pt1 * m_temp_2);
		Pt2 = Pt1 + Pt2;
	}

	SetColor(255, 0, 0);
	DrawLine(Pt1, Pt2);
	


	// Buffer Swap 
	BufferSwap();
}

void DrawLine(Vector3 start, Vector3 dest)
{
	PutPixel(start.X, start.Y);
	PutPixel(dest.X, dest.Y);

	float countX = start.X - dest.X;
	if (countX < 0) countX *= -1;
	float countY = start.Y - dest.Y;
	if (countY < 0) countY *= -1;

	float dotVal = 100 / (countX * countY);
	//float t = 0.5;

	int loopVal = 0;

	for (float t = 0.0f; t < 1; t += dotVal)
	{
		Vector3 tempStart = start;
		Vector3 tempDest = dest;

		float frontP = 1 - t;
		float frontD = t;

		tempStart.X *= frontP;
		tempStart.Y *= frontP;

		tempDest.X *= frontD;
		tempDest.Y *= frontD;

		Vector3 between;
		between.SetPoint(tempStart.X + tempDest.X, tempStart.Y + tempDest.Y);

		SetColor(0, 255, 0);
		PutPixel(between.X, between.Y);
		loopVal++;
	}

	/*
	float length = (dest - start).Dist();

	float inc = 1.0 / length;

	int maxLength = (float)length;

	for (int i = 0; i <= maxLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;

		Vector3 pt = (start * (1.0f) - t) + end * t;
		PutPixel(pt.X, pt.Y);
	}
	*/

}


Vector2 SetScale(Vector2 dotPos, float scaleVal) {

	Matrix2 tempMat(scaleVal, 0, scaleVal, 0);

	return dotPos * tempMat;
}


void DrawCircle(Vector2 drawPos, int r) {

	for (int i = -r ; i < r; i++)
	{
		for (int j = -r; j < r; j++)
		{
			if ((i * i) + (j * j) < (r * r)) {
				PutPixel(i , j );

			}
		}
	}
}

void DrawRectangle(Vector2 leftTop, Vector2 rightBottom) {


	Matrix2 rot;

	static float angle = 0.0f;
	angle += 0.001f;

	rot.SetRotation(angle);
	Vector2 recTemp(5, 5);
	Vector2 NewTemp = recTemp * rot;
	PutPixel(NewTemp.X, NewTemp.Y);

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (i == j)
			{
				Vector2 recTemp(i, j);
				recTemp = recTemp * rot;
				PutPixel(recTemp.X, recTemp.Y);
			}
		}
	}

	for (int i = (int)leftTop.Y; i > (int)rightBottom.Y; i--)
	{
		for (int j = (int)leftTop.X; j < (int)rightBottom.X; j++)
		{
			Vector2 recTemp(i, j);
			recTemp = recTemp * rot;
			PutPixel(recTemp.X, recTemp.Y);
		}
	}
}
