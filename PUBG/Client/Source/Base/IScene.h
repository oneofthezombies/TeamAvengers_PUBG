#pragma once

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
    size_t             m_index;
    std::set<IObject*> m_terrainFeatures;
    std::set<IObject*> m_bullets;
    std::set<IObject*> m_characters;
    std::set<IObject*> m_doors;
    std::set<IObject*> m_windows;
    std::set<IObject*> m_items;

    CellSpace();
};

class IScene : public MemoryAllocator
{
private:
    unordered_set<IObject*>        m_objects;
    unordered_map<IObject*, float> m_toDeleteObjects;

    DirectionalLight* m_pDirectionalLight;
    HeightMap* pHeightMap;

    std::vector<CellSpace>* m_pCells;

protected:
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

    std::size_t GetCellIndex(const D3DXVECTOR3& position);
    void MoveCell(std::size_t currentIndex, std::size_t nextIndex);
    bool IsMovable(const D3DXVECTOR3& targetPos); // 갈 수 있는지
    
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

