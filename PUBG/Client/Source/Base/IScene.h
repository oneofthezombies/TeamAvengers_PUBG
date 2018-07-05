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
    bool              GetHeight(const D3DXVECTOR3 & pos, OUT float * OutHeight);

    bool              isOutOfBoundaryBox(const D3DXVECTOR3& pos);


    //Cell - Space  Partitioning function
    void InsertObjIntoCellSpace(TAG_OBJECT tag, size_t index, IN IObject* obj);
    std::size_t GetCellIndex(const D3DXVECTOR3& position);
    void MoveCell(OUT std::size_t* currentCellIndex, std::size_t destCellIndex, TAG_OBJECT tag, IObject* obj);
    bool IsMovable(const D3DXVECTOR3* targetPos, size_t currentCellIndex, TAG_OBJECT tag, IObject* obj);




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

