#include "stdafx.h"
#include "GlobalDefinition.h"

WORD g_aCubeIndex[36] = {
	0, 1, 2, 0, 2, 3,	// 후
	7, 6, 5, 7, 5, 4,	// 전
	4, 5, 1, 4, 1, 0,	// 좌
	3, 2, 6, 3, 6, 7,	// 우
	1, 5, 6, 1, 6, 2,	// 상
	4, 0, 3, 4, 3, 7	// 하
};

D3DXVECTOR3 g_aCubeVertex[8] = {
	D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(-1.0f,  1.0f, -1.0f),
	D3DXVECTOR3(1.0f,  1.0f, -1.0f),
	D3DXVECTOR3(1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(-1.0f, -1.0f,  1.0f),
	D3DXVECTOR3(-1.0f,  1.0f,  1.0f),
	D3DXVECTOR3(1.0f,  1.0f,  1.0f),
	D3DXVECTOR3(1.0f, -1.0f,  1.0f)
};

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

float GetRandomFloat(float lowBound, float highBound)
{
	float f = (rand() % 10000)*0.0001f; //0에서 1까지 clamp 해준 것이다
	//f = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return (f*(highBound - lowBound)) + lowBound;
}

void GetRotationMatrixFromDirection(D3DXMATRIX& OutRotation, const D3DXVECTOR3& direction, const D3DXVECTOR3& up)
{
    D3DXVECTOR3 xAxis, yAxis;
    D3DXVec3Cross(&xAxis, &up, &direction);
    D3DXVec3Normalize(&xAxis, &xAxis);

    D3DXVec3Cross(&yAxis, &direction, &xAxis);
    D3DXVec3Normalize(&yAxis, &yAxis);

    D3DXMatrixIdentity(&OutRotation);
    OutRotation._11 = xAxis.x;      OutRotation._12 = xAxis.y;      OutRotation._13 = xAxis.z;
    OutRotation._21 = yAxis.x;      OutRotation._22 = yAxis.y;      OutRotation._23 = yAxis.z;
    OutRotation._31 = direction.x;  OutRotation._32 = direction.y;  OutRotation._33 = direction.z;
}

string CStringToStdString(const CString val)
{
    return string(CT2CA(val));
}
