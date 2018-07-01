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
    m_dimension = dimension;
    m_numTile = m_dimension - 1;
}

const vector<D3DXVECTOR3>& IMap::GetVertices() const
{
    return m_vecVertex;
}
