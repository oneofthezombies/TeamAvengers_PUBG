#include "stdafx.h"
#include "IMap.h"

IMap::IMap()
    : IObject()
{
}

IMap::~IMap()
{
}

void IMap::SetDimension(const int dimension)
{
    m_Dimension = dimension; 
    m_NumTile = m_Dimension - 1;
}

const vector<D3DXVECTOR3>& IMap::GetVertices() const
{
    return m_Vertices;
}