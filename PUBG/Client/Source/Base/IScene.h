#pragma once

#define CellSpaceDim 4

class IObject;
class DirectionalLight;
class HeightMap;

struct BoxColliderInFile
{
    D3DXMATRIX  m_transform;

    BoxColliderInFile();
};

struct ObjectInFile
{
    TAG_RES_STATIC m_tagResStatic;
    std::string    m_name;
    D3DXVECTOR3    m_position;
    D3DXVECTOR3    m_rotation;
    D3DXVECTOR3    m_scale;

    std::vector<BoxColliderInFile> m_boxColliders;
    
    ObjectInFile();
};

struct CellSpace
{
    size_t             pIndex;
    std::set<IObject*> pTerrainFeatures;
    std::set<IObject*> pBullets;
    std::set<IObject*> pCharacters;
    std::set<IObject*> pDoors;
    std::set<IObject*> pWindows;
    std::set<IObject*> pItems;

    CellSpace(size_t index);
};

class IScene : public MemoryAllocator
{
private:
    unordered_set<IObject*>        m_objects;
    unordered_map<IObject*, float> m_toDeleteObjects;

    DirectionalLight* m_pDirectionalLight;

protected:
    HeightMap * pHeightMap;
    std::vector<CellSpace*> m_pCellSpaces;
    
    IScene();

	void updateToDeleteObjects();
    HRESULT parseObjectInFile(std::ifstream& fin, ObjectInFile* Out);
    HRESULT parseBoxColliderInFile(
        std::ifstream& fin, BoxColliderInFile* Out);

public:
    virtual ~IScene();
	
	void Init();
	void Update();
	void Render();

    void AddObject(IObject* p);
    void RemoveObject(IObject* p);
	void Destroy(IObject* p, const float t = 0.0f);

    void LoadObjectsFromFile(const std::string& fullPath);
    IObject* FindWithTag(const TAG_OBJECT tag);

    void              SetDirectionalLight(DirectionalLight* p);
    DirectionalLight* GetDirectionalLight();

    void              SetHeightMap(HeightMap* p);
    HeightMap*        GetHeightMap();
    bool GetHeight(const D3DXVECTOR3 & pos, OUT float * OutHeight);

    //Cell - Space  Partitioning 
    void InsertObjIntoCellSpace(TAG_OBJECT tag, size_t index,IN IObject* obj)
    {
        switch (tag)
        {
        case TAG_OBJECT::Idle:
            break;
        case TAG_OBJECT::TerrainFeature:
            m_pCellSpaces[index]->pTerrainFeatures.emplace(obj);
            break;
        case TAG_OBJECT::Bullet:
            m_pCellSpaces[index]->pBullets.emplace(obj);
            break;
        case TAG_OBJECT::Character:
            m_pCellSpaces[index]->pCharacters.emplace(obj);
            break;
        case TAG_OBJECT::Door:
            m_pCellSpaces[index]->pDoors.emplace(obj);
            break;
        case TAG_OBJECT::Window:
            m_pCellSpaces[index]->pWindows.emplace(obj);
            break;
        case TAG_OBJECT::Item:
            m_pCellSpaces[index]->pItems.emplace(obj);
            break;
        }
        
    }
    std::size_t GetCellIndex(const D3DXVECTOR3& position);
    
    void MoveCell(OUT std::size_t* currentCellIndex, std::size_t destCellIndex,TAG_OBJECT tag,IObject* obj)
    {
        switch (tag)
        {
        case TAG_OBJECT::Bullet:
            //총알에 대하여 만들기
            break;
        case TAG_OBJECT::Character:
            //object를 찾는다.
            auto itr = m_pCellSpaces[*currentCellIndex]->pCharacters.find(obj);
            if (itr == m_pCellSpaces[*currentCellIndex]->pCharacters.end())
                assert(false && "MoveCell() cannot find obj");
            //찾고 원래 장소에서 지운다
            IObject* ptr = *itr;
            m_pCellSpaces[*currentCellIndex]->pCharacters.erase(itr);
            //새 장소에 넣어준다
            m_pCellSpaces[destCellIndex]->pCharacters.emplace(ptr);
            //현재 인덱스를 바꾸어준다
            *currentCellIndex = destCellIndex;
            break;
        }
    }
    
    bool IsMovable(const D3DXVECTOR3* targetPos,size_t currentCellIndex,TAG_OBJECT tag, IObject* obj)// 갈 수 있는지
    {
        


        return false;
    } 
    
    virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;

    template<
        typename T, 
        std::enable_if_t<std::is_base_of_v<IObject, T>, int> = 0>
    T* FildWithTagAndCast(const TAG_OBJECT tag)
    {
        for (auto o : m_objects)
        {
            if (o->GetTagObject() == tag)
                return static_cast<T*>(o);
        }

        return nullptr;
    }
};

