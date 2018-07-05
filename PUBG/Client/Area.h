#pragma once
class CellSpace;
class Area : public MemoryAllocator
{
private:
    std::vector<CellSpace*> m_cellspaces;
    size_t m_currentCellSpaceIndex;

public:
    Area();
    ~Area();

    void SetCurrentCellSpaceIndex(size_t index);
    void GetNearArea(size_t index);

    std::vector<IObject*> GetBullets();
    std::vector<IObject*> GetTerrainFeatures();

};

//dest는 near area pos 총 9칸,
//rendering 할때는 frustumArea가 필요  x 와 z로 만드는 