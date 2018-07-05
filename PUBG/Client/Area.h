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

//dest�� near area pos �� 9ĭ,
//rendering �Ҷ��� frustumArea�� �ʿ�  x �� z�� ����� 