#include "stdafx.h"
#include "Grid.h"

Grid::Grid()
    : IObject()
{
    Init();
}

Grid::~Grid()
{
}

void Grid::Init()
{
    //D3DXMatrixIdentity(&m_transformationMatrix);
    D3DXMatrixScaling(&m_transformationMatrix, 100, 0, 100);

    pEffect = Resource()()->GetEffect("./Resource/", "Color.fx");

	float interval = 1.0f;
	int numHalfLine = 15;
	float halfLength = interval * numHalfLine;
	float tmpInterval;
	for (int i = 1; i <= numHalfLine; i++)
	{
		tmpInterval = interval * i;
        if (i % 5 == 0)
        {
            m_whites.push_back(D3DXVECTOR3(-halfLength, 0.0f, tmpInterval));
            m_whites.push_back(D3DXVECTOR3(halfLength, 0.0f, tmpInterval));
            m_whites.push_back(D3DXVECTOR3(-halfLength, 0.0f, -tmpInterval));
            m_whites.push_back(D3DXVECTOR3(halfLength, 0.0f, -tmpInterval));
            m_whites.push_back(D3DXVECTOR3(tmpInterval, 0.0f, -halfLength));
            m_whites.push_back(D3DXVECTOR3(tmpInterval, 0.0f, halfLength));
            m_whites.push_back(D3DXVECTOR3(-tmpInterval, 0.0f, -halfLength));
            m_whites.push_back(D3DXVECTOR3(-tmpInterval, 0.0f, halfLength));
        }
        else
        {
            m_grays.push_back(D3DXVECTOR3(-halfLength, 0.0f, tmpInterval));
            m_grays.push_back(D3DXVECTOR3(halfLength, 0.0f, tmpInterval));
            m_grays.push_back(D3DXVECTOR3(-halfLength, 0.0f, -tmpInterval));
            m_grays.push_back(D3DXVECTOR3(halfLength, 0.0f, -tmpInterval));
            m_grays.push_back(D3DXVECTOR3(tmpInterval, 0.0f, -halfLength));
            m_grays.push_back(D3DXVECTOR3(tmpInterval, 0.0f, halfLength));
            m_grays.push_back(D3DXVECTOR3(-tmpInterval, 0.0f, -halfLength));
            m_grays.push_back(D3DXVECTOR3(-tmpInterval, 0.0f, halfLength));
        }
	}

    m_reds.push_back(D3DXVECTOR3(-halfLength, 0.0f, 0.0f));
    m_reds.push_back(D3DXVECTOR3( halfLength, 0.0f, 0.0f));

    m_greens.push_back(D3DXVECTOR3(0.0f, -halfLength, 0.0f));
    m_greens.push_back(D3DXVECTOR3(0.0f,  halfLength, 0.0f));

    m_blues.push_back(D3DXVECTOR3(0.0f, 0.0f, -halfLength));
    m_blues.push_back(D3DXVECTOR3(0.0f, 0.0f,  halfLength));
}

void Grid::OnUpdate()
{
}

void Grid::OnRender()
{
    draw(m_grays, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
    draw(m_whites, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    draw(m_reds, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

    // TODO : where is green line?
    draw(m_greens, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

    draw(m_blues, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
}

void Grid::draw(const vector<D3DXVECTOR3>& vertices, const D3DXCOLOR& color)
{
    Shader::Draw(
        pEffect,
        nullptr,
        [this, &color](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m_transformationMatrix);
        pEffect->SetValue("Color", &color, sizeof color);
    },
        [this, &vertices]()
    {
        Device()()->DrawPrimitiveUP(
            D3DPT_LINELIST,
            vertices.size() / 2,
            vertices.data(),
            sizeof vertices.front());
    });
}