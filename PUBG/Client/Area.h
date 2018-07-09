#pragma once

struct CellSpace;
class TerrainFeature;

class Area : public MemoryAllocator
{
private:
    std::vector<CellSpace*> m_cellspaces;

public:
    Area();
    ~Area();

    void Create(const std::size_t index);

    std::vector<IObject*> GetBullets();
    std::vector<TerrainFeature*> GetTerrainFeatures();
    std::vector<Item*> GetItems();
    //void ItemIntoInventory(size_t index, Item* obj);
    bool checkValid(int v);
};

//dest�� near area pos �� 9ĭ,
//rendering �Ҷ��� frustumArea�� �ʿ�  x �� z�� ����� 