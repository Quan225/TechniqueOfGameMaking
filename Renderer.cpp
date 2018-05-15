
#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"



struct DrawData {
public:
	DrawData() {}
	DrawData(Mesh* mesh, int _layer) {
		m = mesh;
		layer = _layer;
	}

	Mesh * m;
	int layer;
};


bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return; // Culling

	ULONG* dest = (ULONG*)g_pBits;

	int halfWidth = RoundToInt(g_nClientWidth * 0.5f);
	int halfHeight = RoundToInt(g_nClientHeight * 0.5f);

	DWORD offset = (halfHeight * g_nClientWidth - g_nClientWidth * pt.Y) + (halfWidth + pt.X);
	*(dest + offset) = g_CurrentColor;
}

void DrawCall (Mesh * MeshToDraw, Vector2 worldPosition, float angle, float scale)
{
	V2F_CUSTOM* v2f = new V2F_CUSTOM [ MeshToDraw->VSize ]; // Mesh�� Vertex��ŭ ������ �Ҵ�


	for (int i = 0; i < 4; i++)
	{
		v2f[i].position = MeshToDraw->Vertices[i].position;
		v2f[i].uv = MeshToDraw->Vertices[i].uv;
		v2f[i].color = MeshToDraw->Vertices[i].color;
	}


// ===========================

	Matrix3 TMat, RMat, SMat;


	// World ��ȯ 
	Matrix3 worldMatrix;	// worldMatrix ����
	TMat.SetTranslation(worldPosition.X, worldPosition.Y);	// ���� ������ ��
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	worldMatrix = TMat * RMat * SMat;

	for (int i = 0; i < MeshToDraw->VSize; i++)	
	{
		v2f[i].position = v2f[i].position * worldMatrix;	// �� Vertex�� �����Ѵ�.
	}



	// View ��ȯ
	Matrix3 viewMatrix;	// viewMatrix ����

	Vector3 cameraPosition(10.0f, 10.0f, 1.0f);
	float cameraAngle = 10.0f;

	//Vector3 tempCamUpVec(0, 1.0f, 0); // ���� Up Vector�̴�. X���� ���ϴµ� ��� (ī�޶��� ȸ���� �̰����� �����غ��� �� ��)
	//Vector3 camZAxis(0, 0, 1.0f); // 2D�ϱ� ������ ������ �ٶ󺻴�.
	//Vector3 camXAxis = tempCamUpVec.Cross(camZAxis); // Up �� Z�� �����Ͽ� X�� ���Ѵ�.
	//Vector3 camYAxis = camZAxis.Cross(camXAxis); // Z�� X�� �����Ͽ� Y�� ���Ѵ�.

	// �׳� �̰Ÿ� �ǰڴ�.
	TMat.SetTranslation(-cameraPosition.X, -cameraPosition.Y);	// ī�޶��� ��ġ�� �°� �̵���Ų��.
	RMat.SetRotation(-cameraAngle);
	SMat.SetScale(cameraPosition.Z); 
	viewMatrix = TMat * RMat * SMat;
										
	for (int i = 0; i < MeshToDraw->VSize; i++)
	{
		v2f[i].position = v2f[i].position * viewMatrix;	// �� Vertex�� �����Ѵ�.
	}

	

	for ( int i = 0; i < MeshToDraw->VSize; i++ )
	{
		APPDATA_CUSTOM vdata;	// ������ �����͸� ������ ������ �����
		vdata.position = v2f[i].position; // �����͸� �Է��� ��
		vdata.color = v2f[i].color;
		vdata.uv = v2f[i].uv;
		v2f[i] = VertexShader ( vdata ); // ���ؽ� ���̴� ����
	}

	for ( int i = 0; i < MeshToDraw->ISize; i+=3 )
	{
		Triangle t ( v2f [ MeshToDraw->Indices[i]], 
			v2f [ MeshToDraw->Indices[i + 1]], 
			v2f [ MeshToDraw->Indices[i + 2]] );

		// Rasterization
		for ( int y = t.Min.Y; y <= t.Max.Y; y++ )
		{
			for ( int x = t.Min.X; x <= t.Max.X; x++ )
			{
				Vector3 target ( ( float ) x + 0.5f, ( float ) y + 0.5f, 0.0f );
				float outS, outT;
				t.CalcBaryCentricCoord ( target, &outS, &outT );
				if ( t.IsInTrianble ( outS, outT ) )
				{
					V2F_CUSTOM v2f;
					v2f.position = t.GetFragmentPos ( target, outS, outT );
					v2f.color = t.GetFragmentColor ( target, outS, outT );
					v2f.uv = t.GetFragmentUV ( target, outS, outT );
					ULONG finalColor = FragmentShader ( v2f, outS, outT );
					g_CurrentColor = finalColor;
					PutPixel ( IntPoint(x, y) );
				}
			}
		}
	}

	if ( NULL != v2f )
	{
		delete [ ] v2f;
	}
}

V2F_CUSTOM VertexShader ( APPDATA_CUSTOM in )
{
	// TRS Matrix �ݿ��ؼ� ��ȯ�� position �����ֱ�..
	V2F_CUSTOM retTemp(in);
	Matrix3 vertexMat;
	Matrix3 tempT;
	Matrix3 tempR;
	Matrix3 tempS;

	tempT.SetTranslation(retTemp.position.X * 0.25f, 0.00f);
	tempR.SetRotation(20.0f);
	tempS.SetScale(1.0f);
	
	vertexMat = tempT * tempR * tempS;

	retTemp.position = retTemp.position * vertexMat;

	return retTemp;
}

ULONG FragmentShader ( V2F_CUSTOM in, float s, float t )
{
	// Texture���� ���� ������..
	if ( g_Texture->IsLoaded ( ) )
	{
		return g_Texture->TextureSample ( in.uv, s, t );
	}

	return in.color;
}

//void DrawLine(const Vector3& start, const Vector3& end)
//{
//	float length = (end - start).Dist();
//	float inc = 1.0f / length;
//
//	int maxLength = RoundToInt(length);
//	for (int i = 0; i <= maxLength; i++)
//	{
//		float t = inc * i;
//		if (t >= length) t = 1.0f;
//		Vector3 Pt = start * (1.0f - t) + end * t;
//		PutPixel(Pt.ToIntPoint());
//	}
//}

//void Draw2DTriangle(Triangle t)
//{
//	for (int y = t.Min.Y; y <= t.Max.Y; y++)
//	{
//		for (int x = t.Min.X; x <= t.Max.X; x++)
//		{
//			Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
//			float outS, outT;
//			t.CalcBaryCentricCoord(target, &outS, &outT);
//			if (t.IsInTrianble(outS, outT))
//			{
//				if (g_Texture->IsLoaded())
//				{
//					g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, t);
//				}
//				else
//				{
//					g_CurrentColor = t.GetPixelColor(target, outS, outT);
//				}
//
//				PutPixel(IntPoint(x, y));
//			}			
//		}
//	}
//}

void InitFrame ( void )
{
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	Vector3 Pt1, Pt2, Pt3, Pt4;
	Vector3 Pt5, Pt6, Pt7, Pt8;


	Pt1.SetPoint(-50, 50.0f);
	Pt2.SetPoint(50.0f, 50.0f);
	Pt3.SetPoint(50.0f, -50.0f);
	Pt4.SetPoint(-50.0f, -50.0f);


	Pt5.SetPoint(-100, -25.0f);
	Pt6.SetPoint(-0.0f, -25.0f);
	Pt7.SetPoint(-0.0f, -125.0f);
	Pt8.SetPoint(-100.0f, -125.0f);



	static float offsetX = 0.0f;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) offsetX -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) offsetX += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;
	
	Vector2 worldPos(offsetX, 0.0f);


	Mesh* m1 = new Mesh();
	Mesh* m2 = new Mesh();
	Vertex* vt1 = new Vertex[4];
	Vertex* vt2 = new Vertex[4];

	vt1[0].position = Pt1;
	vt1[0].color = RGB32(255, 0, 0);
	vt1[0].uv = Vector2(0.125f, 0.125f);
	vt1[1].position = Pt2;
	vt1[1].color = RGB32(0, 255, 0);
	vt1[1].uv = Vector2(0.25f, 0.125f);
	vt1[2].position = Pt3;
	vt1[2].color = RGB32(0, 0, 255);
	vt1[2].uv = Vector2(0.25f, 0.25f);
	vt1[3].position = Pt4;
	vt1[3].color = RGB32(255, 255, 0);
	vt1[3].uv = Vector2(0.125f, 0.25f);


	int* idx = new int[6];
	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 0;
	idx[4] = 3;
	idx[5] = 2;

	m1->SetVertetices(vt1, 4);
	m1->SetIndices(idx, 6);



	vt2[0].position = Pt5;
	vt2[1].position = Pt6;
	vt2[2].position = Pt7;
	vt2[3].position = Pt8;


	m2->SetVertetices(vt2, 4);
	m2->SetIndices(idx, 6);

	static int layerTestNum;

	DrawData dd[2];
	dd[0].m = m1;
	dd[0].layer = 0;
	dd[1].m = m2;
	dd[1].layer = 2;

	if (GetAsyncKeyState(VK_NUMPAD1)) dd[0].layer = 3;



	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (dd[j].layer == i) {
				DrawCall(dd[j].m, worldPos, angle, scale);
			}
		}
	}


	//DrawCall(m1, worldPos, angle, scale, camOffsetX);


	//DrawCall(m2, worldPos, angle, scale, camOffsetX);

	// Buffer Swap 
	BufferSwap();
}
