#pragma once

struct CellSpace;
class TerrainFeature;
class Character;

class Area : public MemoryAllocator
{
private:
    std::vector<CellSpace*> m_cellspaces;

public:
    Area();
    ~Area();

    void CreateNearArea(const std::size_t index);
    Ray CreateRayArea(Ray* ray,const float rayLength);

    std::vector<Bullet*>         GetBullets();
    std::vector<TerrainFeature*> GetTerrainFeatures();
    std::vector<Character*>      GetCharacters();
    std::vector<Item*> GetItems();
    //void ItemIntoInventory(size_t index, Item* obj);
    bool checkValid(int v);
};

//dest는 near area pos 총 9칸,
//rendering 할때는 frustumArea가 필요  x 와 z로 만드는 