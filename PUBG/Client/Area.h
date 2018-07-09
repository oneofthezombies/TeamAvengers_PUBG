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

    void Create(const std::size_t index);

    std::vector<Bullet*>         GetBullets();
    std::vector<TerrainFeature*> GetTerrainFeatures();
    std::vector<Character*>      GetCharacters();
    bool checkValid(int v);
};

//dest는 near area pos 총 9칸,
//rendering 할때는 frustumArea가 필요  x 와 z로 만드는 