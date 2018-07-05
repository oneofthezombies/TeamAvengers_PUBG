#include "stdafx.h"
#include "IScene.h"
#include "TerrainFeature.h"



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

void Area::GetNearArea(size_t index)
{
    vector<CellSpace>* CS = CurrentScene()()->GetTotalCellSpace();

    


    m_cellspaces.emplace_back(&CS->at(index));
}

std::vector<IObject*> Area::GetBullets()
{
    std::vector<IObject*> vBullets;
    for (auto p : m_cellspaces)
    {
        vBullets.emplace_back(p->pBullets);
    }

    return vBullets;
}

std::vector<IObject*> Area::GetTerrainFeatures()
{
    std::vector<IObject*> vTerrainFeatures;
    for (auto p : m_cellspaces)
    {
        vTerrainFeatures.emplace_back(p->pTerrainFeatures);
    }
    
    return vTerrainFeatures;
}
