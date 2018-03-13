
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y); 
void DrawCircle(int x, int y, int r);

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


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	PutPixel(0, 0);

	DrawCircle(10, 100, 40);

	// Buffer Swap 
	BufferSwap();
}


void DrawCircle(int x, int y, int r) {


	for (int i = -r ; i < r; i++)
	{
		for (int j = -r; j < r; j++)
		{
			if ((i * i) + (j * j) < (r * r)) {
				PutPixel(i + x, j + y);

			}
		}
	}
}
