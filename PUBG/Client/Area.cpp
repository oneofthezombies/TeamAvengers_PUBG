#include "stdafx.h"
#include "IScene.h"

Area::Area()
{
}

Area::~Area()
{
}

void Area::CreateNearArea(const std::size_t index)
{
    m_cellspaces.clear();

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

Ray Area::CreateRayArea(Ray* ray, const float rayLength)
{
    m_cellspaces.clear();
    vector<CellSpace>* CS = CurrentScene()()->GetTotalCellSpace();

    //Ray ray = Ray::RayAtWorldSpace(1280 / 2, 720 / 2);

    const float cellLength = CurrentScene()()->GetCellSpaceLength();
    BoundingRect rect;
    rect.center.x = rect.center.y = cellLength / 2;
    rect.extent = rect.center;
    for (int i = 0; i <CellSpace::DIMENSION; i++)
    {
        for (int j = 0; j < CellSpace::DIMENSION; j++)
        {
            rect.position = D3DXVECTOR2(j*cellLength, i*cellLength);
            if (Collision::HasCollision(*ray, rect, rayLength))
            {
                m_cellspaces.emplace_back(&CS->at(j + i * CellSpace::DIMENSION));

            }
        }
    }

    return *ray;
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

std::vector<Item*> Area::GetItems()
{
    std::vector<Item*> vItems;
    for (std::size_t i = 0; i < m_cellspaces.size(); i++)
    {
        auto& set = m_cellspaces[i]->pItems;
        vItems.insert(vItems.end(), set.begin(), set.end());
    }
    return vItems;
}

//void Area::ItemIntoInventory(size_t index, Item * obj)
//{
//    auto search = m_cellspaces[index]->pItems.find(obj);
//    if (search != m_cellspaces[index]->pItems.end())
//        assert(false && "Area::ItemIntoInventory(), cannot find Item ");
//
//    // Cell Space 에서 item을 빼서 inventory에 넣는다.
//    m_cellspaces[index]->pItems.erase(obj); 
//}

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
