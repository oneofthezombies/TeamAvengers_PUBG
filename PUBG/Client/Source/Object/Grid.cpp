#include "stdafx.h"
#include "Grid.h"


Grid::Grid()
{
    Init();
}


Grid::~Grid()
{
}

void Grid::Init()
{
	float interval = 1.0f;
	int numHalfLine = 15;
	float halfLength = interval * numHalfLine;
	D3DCOLOR c;
	float tmpInterval;
	for (int i = 1; i <= numHalfLine; i++)
	{
		tmpInterval = interval * i;
		c = (i % 5 == 0) ? D3DCOLOR_XRGB(255, 255, 255) :
			D3DCOLOR_XRGB(128, 128, 128);
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3(-halfLength, 0.0f,  tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3( halfLength, 0.0f,  tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3(-halfLength, 0.0f, -tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3( halfLength, 0.0f, -tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3( tmpInterval, 0.0f, -halfLength), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3( tmpInterval, 0.0f,  halfLength), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3(-tmpInterval, 0.0f,  -halfLength), c));
		m_vecVertex.push_back(VERTEX_PC(
			D3DXVECTOR3(-tmpInterval, 0.0f,   halfLength), c));
	}
	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecVertex.push_back(VERTEX_PC(
		D3DXVECTOR3(-halfLength, 0.0f, 0.0f), c));
	m_vecVertex.push_back(VERTEX_PC(
		D3DXVECTOR3( halfLength, 0.0f, 0.0f), c));
	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecVertex.push_back(VERTEX_PC(
		D3DXVECTOR3(0.0f, -halfLength, 0.0f), c));
	m_vecVertex.push_back(VERTEX_PC(
		D3DXVECTOR3(0.0f,  halfLength, 0.0f), c));
	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecVertex.push_back(VERTEX_PC(
		D3DXVECTOR3(0.0f, 0.0f, -halfLength), c));
	m_vecVertex.push_back(VERTEX_PC(
		D3DXVECTOR3(0.0f, 0.0f,  halfLength), c));
}

void Grid::Render()
{
	D3DXMATRIX matI;
	//D3DXMatrixIdentity(&matI);
    D3DXMatrixScaling(&matI, 100, 0, 100);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pDevice->SetFVF(VERTEX_PC::FVF);
	g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		m_vecVertex.size() / 2, &m_vecVertex[0], sizeof(VERTEX_PC));
}

void Grid::OnUpdate()
{

}

void Grid::OnRender()
{
    Render();
}
