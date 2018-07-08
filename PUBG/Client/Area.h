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

    std::vector<IObject*> GetBullets();
    std::vector<IObject*> GetTerrainFeatures();
    std::vector<IObject*> GetCharacters();
    bool checkValid(int v);
};

//dest�� near area pos �� 9ĭ,
//rendering �Ҷ��� frustumArea�� �ʿ�  x �� z�� ����� 