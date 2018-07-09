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

//dest는 near area pos 총 9칸,
//rendering 할때는 frustumArea가 필요  x 와 z로 만드는 