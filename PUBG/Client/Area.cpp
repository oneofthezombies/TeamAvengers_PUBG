#include "stdafx.h"
#include "IScene.h"



Area::Area()
{
}

Area::~Area()
{
}

void Area::SetCurrentCellSpaceIndex(size_t index)
{
    m_currentCellSpaceIndex = index;
}
bool checkValid(int v)
{
    if (-1 < v&&v < CellSpace::DIMENSION)
        return true;
    return false;
}
void Area::GetNearArea(size_t index)
{
    vector<CellSpace>* CS = CurrentScene()()->GetTotalCellSpace();

    vector<D3DXVECTOR2> p;
    p.push_back(D3DXVECTOR2(-1, -1));
    p.push_back(D3DXVECTOR2( 0, -1));
    p.push_back(D3DXVECTOR2( 1, -1));
    p.push_back(D3DXVECTOR2(-1,  0));
    p.push_back(D3DXVECTOR2( 1,  0));
    p.push_back(D3DXVECTOR2(-1,  1));
    p.push_back(D3DXVECTOR2( 0,  1));
    p.push_back(D3DXVECTOR2( 1,  1));

    int col = index /CellSpace::DIMENSION;
    int row = index % CellSpace::DIMENSION;

    //for (int i = 0; i < CellSpace::DIMENSION; i++)
    //{
    //    for (int j = 0; j < CellSpace::DIMENSION; j++)
    //    {
    //        3

    //    }

    //}

    //if (row != 0)
    //    m_cellspaces.emplace_back(&CS->at(index - 1));
    //if(CellSpace::DIMENSION-row > 1 )


    //m_cellspaces.emplace_back(&CS->at(index));
    
}

std::vector<IObject*> Area::GetBullets()
{
    std::vector<IObject*> vBullets;
    for (int i = 0; i < m_cellspaces.size(); i++)
    {
        auto& set = m_cellspaces[i]->pBullets;
        vBullets.insert(vBullets.end(), set.begin(), set.end());
    }
    return vBullets;
}

std::vector<IObject*> Area::GetTerrainFeatures()
{
    std::vector<IObject*> vTerrainFeatures;
    for (int i = 0; i < m_cellspaces.size(); i++)
    {
        auto& set = m_cellspaces[i]->pTerrainFeatures;
        vTerrainFeatures.insert(vTerrainFeatures.end(), set.begin(), set.end());
    }
    return vTerrainFeatures;
}
