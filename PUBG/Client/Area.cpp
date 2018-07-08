#include "stdafx.h"
#include "IScene.h"

Area::Area()
{
}

Area::~Area()
{
}

void Area::Create(const std::size_t index)
{
    vector<CellSpace>* CS = CurrentScene()()->GetTotalCellSpace();

    vector<D3DXVECTOR2> p;
    p.push_back(D3DXVECTOR2(-1, -1));
    p.push_back(D3DXVECTOR2(0, -1));
    p.push_back(D3DXVECTOR2(1, -1));

    p.push_back(D3DXVECTOR2(-1, 0));
    p.push_back(D3DXVECTOR2(1, 0));

    p.push_back(D3DXVECTOR2(-1, 1));
    p.push_back(D3DXVECTOR2(0, 1));
    p.push_back(D3DXVECTOR2(1, 1));

    int col = index / CellSpace::DIMENSION;
    int row = index % CellSpace::DIMENSION;

    m_cellspaces.clear();
    int nRow = 0;
    int nCol = 0;
    for (auto& n : p)
    {
        nRow = static_cast<int>(n.x) + row;
        nCol = static_cast<int>(n.y) + col;
        if (checkValid(nRow) && checkValid(nCol))
        {
            m_cellspaces.emplace_back(&CS->at(nRow + nCol * CellSpace::DIMENSION));
        }
    }

    m_cellspaces.emplace_back(&CS->at(row + col * CellSpace::DIMENSION));
}

bool Area::checkValid(int v)
{
    return-1 < v&&v < CellSpace::DIMENSION;
}

std::vector<Bullet*> Area::GetBullets()
{
    std::vector<Bullet*> vBullets;
    for (std::size_t i = 0; i < m_cellspaces.size(); i++)
    {
        auto& set = m_cellspaces[i]->pBullets;
        vBullets.insert(vBullets.end(), set.begin(), set.end());
    }
    return vBullets;
}

std::vector<TerrainFeature*> Area::GetTerrainFeatures()
{
    std::vector<TerrainFeature*> vTerrainFeatures;
    for (std::size_t i = 0; i < m_cellspaces.size(); i++)
    {
        auto& set = m_cellspaces[i]->pTerrainFeatures;
        vTerrainFeatures.insert(vTerrainFeatures.end(), set.begin(), set.end());
    }
    return vTerrainFeatures;
}

std::vector<Character*> Area::GetCharacters()
{
    std::vector<Character*> characters;
    for (auto& cs : m_cellspaces)
    {
        auto& set = cs->pCharacters;
        characters.insert(characters.end(), set.begin(), set.end());
    }
    return characters;
}
