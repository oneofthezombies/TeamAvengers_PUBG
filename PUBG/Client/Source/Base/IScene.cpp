#include "stdafx.h"
#include "IScene.h"
#include "IObject.h"
#include "TerrainFeature.h"
#include "Light.h"
#include "ResourceInfo.h"
#include "Item.h"
#include "HeightMap.h"

BoxColliderInFile::BoxColliderInFile()
{
    D3DXMatrixIdentity(&m_transform);
}

ObjectInFile::ObjectInFile()
    : m_tagResStatic(TAG_RES_STATIC::Bandage)
    , m_name("")
    , m_position(Vector3::ZERO)
    , m_rotation(Vector3::ZERO)
    , m_scale(Vector3::ONE)
{
}

IScene::IScene()
    : MemoryAllocator()
    , pHeightMap(nullptr)
{
}

IScene::~IScene()
{
	for (auto o : m_objects)
        SAFE_DELETE(o);
}

void IScene::Init()
{
    OnInit();
}

void IScene::Update()
{
	for (auto& o : m_objects)
		if (o) o->Update();

	OnUpdate();

	updateToDeleteObjects();
}

void IScene::Render()
{
	for (auto& o : m_objects)
		if (o) o->Render();
}

void IScene::AddObject(IObject* p)
{
    assert(p && "IScene::AddObject(), object is null.");

	m_objects.emplace(p);
}

void IScene::RemoveObject(IObject* p)
{
    assert(p && "IScene::RemoveObject(), object is null.");

	m_objects.erase(p);
}

void IScene::Destroy(IObject* p, const float t)
{
	if (!p) return;

	const auto search = m_toDeleteObjects.find(p);
	if (search != m_toDeleteObjects.end()) return;

	m_toDeleteObjects.emplace(p, t);
}

void IScene::LoadObjectsFromFile(const std::string& fullPath)
{
    std::ifstream fin(fullPath);

    assert(fin.is_open() && 
        "IScene::LoadObjectFromFile(), file path is invalid.");

    string filename;
    fin >> filename;

    string buf;
    fin >> buf >> buf >> buf;   // Num of Object
    int numObjects;
    fin >> numObjects;

    HRESULT hr;
    std::vector<ObjectInFile> objs;
    for (int oi = 0; oi < numObjects; ++oi)
    {
        ObjectInFile obj;
        hr = parseObjectInFile(fin, &obj);
        if (FAILED(hr))
        {
            assert(false && 
                "IScene::LoadObjectsFromFile(), parsing failed.");
        }

        objs.emplace_back(obj);
    }

    for (auto o : objs)
    {
        //cout << static_cast<int>(o.m_tagResStatic) << '\n';
        //cout << o.m_name << '\n';
        //cout << o.m_position << '\n';
        //cout << o.m_rotation << '\n';
        //cout << o.m_scale << '\n';

        //for (auto b : o.m_boxColliders)
        //{
        //    cout << b.m_transform << '\n';
        //}


        //degrees to radians
        o.m_rotation = D3DXToRadian(o.m_rotation);


        if (ResourceInfo::IsItem(o.m_tagResStatic))
        {
            Item* pItem = new Item(o.m_tagResStatic, o.m_position, o.m_rotation, o.m_scale);
            // Total Space 에 Item 넣기
            InsertObjIntoTotalCellSpace(pItem->GetTagObject(), GetCellIndex(o.m_position), pItem);
            AddObject(pItem);
        }
        else
        {
            TerrainFeature* pTerrainFeature = new TerrainFeature(
                o.m_tagResStatic,
                o.m_position,
                o.m_rotation,
                o.m_scale);

            for (auto b : o.m_boxColliders)
                pTerrainFeature->AddBoundingBox(b.m_transform);

            AddObject(pTerrainFeature);
        }

    }
}

IObject* IScene::FindWithTag(const TAG_OBJECT tag)
{
    for (auto o : m_objects)
    {
        if (o->GetTagObject() == tag)
            return o;
    }

    return nullptr;
}

void IScene::updateToDeleteObjects()
{
	const float dt = Time()()->GetDeltaTime();

	const auto begin = m_toDeleteObjects.begin();
	const auto end = m_toDeleteObjects.end();
	for (auto it = begin; it != end;)
	{
		auto p = it->first;
		auto& t = it->second;

		t -= dt;
		if (t > 0.0f)
		{
			++it;
		}
		else
		{
			RemoveObject(p);
            SAFE_DELETE(p);
			it = m_toDeleteObjects.erase(it);
		}
	}
}

HRESULT IScene::parseObjectInFile(std::ifstream& fin, ObjectInFile* Out)
{
    assert(Out && "IScene::parseObjectInFile(), out is null.");

    string buf;
    fin >> buf;
    if (buf != "{")
    {
        return E_FAIL;
    }

    fin >> buf;
    Out->m_tagResStatic = static_cast<TAG_RES_STATIC>(std::stoi(buf));

    std::getline(fin >> std::ws, buf);
    Out->m_name = buf;

    fin >> Out->m_position.x >> Out->m_position.y >> Out->m_position.z
        >> Out->m_rotation.x >> Out->m_rotation.y >> Out->m_rotation.z
        >> Out->m_scale.   x >> Out->m_scale.   y >> Out->m_scale.   z;

    fin >> buf;
    if (buf == "}")
    {
        return S_OK;
    }
    else if (buf != "[")
    {
        return E_FAIL;
    }

    HRESULT hr;
    fin >> buf; 
    for (int ci = 0; ci < std::stoi(buf); ++ci)
    {
        BoxColliderInFile box;
        hr = parseBoxColliderInFile(fin, &box);
        if (FAILED(hr))
        {
            return E_FAIL;
        }

        Out->m_boxColliders.emplace_back(box);
    }

    fin >> buf;
    if (buf != "]")
    {
        return E_FAIL;
    }

    fin >> buf;
    if (buf != "}")
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT IScene::parseBoxColliderInFile(
    std::ifstream& fin, BoxColliderInFile* Out)
{
    assert(Out && "IScene::parseBoxColliderInFile(), out is null.");

    string buf;
    fin >> buf;
    if (buf != "<")
    {
        return E_FAIL;
    }

    std::getline(fin >> std::ws, buf);

    fin >> Out->m_transform._11 >> Out->m_transform._12
        >> Out->m_transform._13 >> Out->m_transform._14
        >> Out->m_transform._21 >> Out->m_transform._22
        >> Out->m_transform._23 >> Out->m_transform._24
        >> Out->m_transform._31 >> Out->m_transform._32
        >> Out->m_transform._33 >> Out->m_transform._34
        >> Out->m_transform._41 >> Out->m_transform._42
        >> Out->m_transform._43 >> Out->m_transform._44;

    fin >> buf;
    if (buf != ">")
    {
        return E_FAIL;
    }

    return S_OK;
}

void IScene::SetHeightMap(HeightMap* p)
{
    assert(p && "IScene::SetHeightMap(), SetHeightMapis null.");
    m_objects.emplace(p);
    pHeightMap = p;
}

HeightMap * IScene::GetHeightMap()
{
    return pHeightMap;
}

bool IScene::GetHeight(const D3DXVECTOR3 & pos, OUT float * OutHeight)
{
    return pHeightMap->GetHeight(pos, OutHeight);
}

bool IScene::isOutOfBoundaryBox(const D3DXVECTOR3& pos)
{
   
    return  pHeightMap->isOutOfBoundaryBox(pos);
}

//------------------ Cell Space Partitioning--------------------


std::vector<CellSpace>* IScene::GetTotalCellSpace()
{
    return &m_TotalCellSpaces;
}

void IScene::InsertObjIntoTotalCellSpace(TAG_OBJECT tag, size_t index, IN IObject* obj)
{
    switch (tag)
    {
    case TAG_OBJECT::Idle:
        break;
    case TAG_OBJECT::TerrainFeature:
        m_TotalCellSpaces[index].pTerrainFeatures.emplace(static_cast<TerrainFeature*>(obj));
        break;
    //case TAG_OBJECT::Bullet:
    //    m_TotalCellSpaces[index].pBullets.emplace(static_cast<Bullet*>(obj));
    //    break;
    case TAG_OBJECT::Character:
        m_TotalCellSpaces[index].pCharacters.emplace(static_cast<Character*>(obj));
        break;
    case TAG_OBJECT::Door:
        m_TotalCellSpaces[index].pDoors.emplace(obj);
        break;
    case TAG_OBJECT::Window:
        m_TotalCellSpaces[index].pWindows.emplace(obj);
        break;
    case TAG_OBJECT::Item:
        m_TotalCellSpaces[index].pItems.emplace(static_cast<Item*>(obj));
        break;
    }

}

std::size_t IScene::GetCellIndex(const D3DXVECTOR3 & position)
{
    D3DXVECTOR4 MinMax = GetHeightMap()->GetMinMax();

    float Xspace = (MinMax.z - MinMax.x)/ CellSpace::DIMENSION;
    float Zspace = (MinMax.w - MinMax.y)/ CellSpace::DIMENSION;

    int Xindex = static_cast<int>(position.x / Xspace);
    int Zindex = static_cast<int>(position.z / Zspace);
    
    return Zindex * CellSpace::DIMENSION + Xindex;
}

CellSpace * IScene::GetCellSpaceByPosition(const D3DXVECTOR3 & position)
{
    return &m_TotalCellSpaces[GetCellIndex(position)];
}

const float IScene::GetCellSpaceLength()
{
    D3DXVECTOR4 MinMax = GetHeightMap()->GetMinMax();
    return (MinMax.z - MinMax.x) / CellSpace::DIMENSION;
}

void IScene::MoveCell(OUT std::size_t * currentCellIndex, std::size_t destCellIndex, TAG_OBJECT tag, IObject* obj)
{
    auto& itr = m_TotalCellSpaces[*currentCellIndex];
    auto& itrDest = m_TotalCellSpaces[destCellIndex];

    switch (tag)
    {
    //case TAG_OBJECT::Bullet:
    //{
    //    //object를 찾는다.
    //    //auto ptr = *itr.pBullets.find(static_cast<Bullet*>(obj));
    //    //if (!ptr)
    //    //    assert(false && "movecall() cannot find bullet obj");
    //    //찾고 원래 장소에서 지운다
    //    Bullet* pBullet = static_cast<Bullet*>(obj);
    //    itr.pBullets.erase(pBullet);
    //    itrDest.pBullets.emplace(pBullet);
    //    //현재 인덱스를 바꾸어준다
    //    *currentCellIndex = destCellIndex;
    //}
    //    break;

    case TAG_OBJECT::Character:
    {
        //object를 찾는다.
        //auto ptr = *itr.pCharacters.find(static_cast<Character*>(obj));
        //if (!ptr)
        //    assert(false && "movecall() cannot find bullet obj");
        //찾고 원래 장소에서 지운다
        Character* pCharacter = static_cast<Character*>(obj);
        itr.pCharacters.erase(pCharacter);
        //새 장소에 넣어준다
        itrDest.pCharacters.emplace(pCharacter);
        //현재 인덱스를 바꾸어준다
        *currentCellIndex = destCellIndex;
    }
        break;
    }
}

void IScene::ItemIntoInventory(size_t index, Item * obj)
{
    auto search = m_TotalCellSpaces[index].pItems.find(obj);
    if (search == m_TotalCellSpaces[index].pItems.end())
        assert(false && "Area::ItemIntoInventory(), cannot find Item ");
    // Total Cell Space 에서 item을 빼서 inventory에 넣는다.
    m_TotalCellSpaces[index].pItems.erase(obj);
}

//bool IScene::IsMovable(const D3DXVECTOR3 * targetPos, size_t currentCellIndex, TAG_OBJECT tag, IObject * obj)// 갈 수 있는지
//{
//
//
//
//    return false;
//}
