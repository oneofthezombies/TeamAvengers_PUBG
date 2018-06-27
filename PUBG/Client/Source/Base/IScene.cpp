#include "stdafx.h"
#include "IScene.h"
#include "IObject.h"
#include "CollidableStaticObject.h"

BoxColliderInFile::BoxColliderInFile()
    : m_center(Vector3::ZERO)
    , m_extent(Vector3::ONE)
{
    D3DXMatrixIdentity(&m_transform);
}

ObjectInFile::ObjectInFile()
    : m_ID(-1)
    , m_tagResStatic(TAG_RES_STATIC::Rock_1)
    , m_position(Vector3::ZERO)
    , m_rotation(Vector3::ZERO)
    , m_scale(Vector3::ONE)
{
}

IScene::IScene()
    : MemoryAllocator()
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

void IScene::LoadObjectsFromFile(const string& fullPath)
{
    std::ifstream fin(fullPath);

    assert(fin.is_open() && 
        "IScene::LoadObjectFromFile(), file path is invalid.");

    string filename;
    fin >> filename;

    int numObjects;
    fin >> numObjects;

    HRESULT hr;
    vector<ObjectInFile> objs;
    string buf;
    for (int oi = 0; oi < numObjects; ++oi)
    {
        ObjectInFile obj;
        hr = parseObjectInFile(fin, &obj);
        if (FAILED(hr))
        {

        }

        objs.emplace_back(obj);
    }

    for (auto o : objs)
    {
        CollidableStaticObject* pObj = 
            new CollidableStaticObject(
                o.m_tagResStatic, 
                o.m_position, 
                o.m_rotation, 
                o.m_scale);

        for (auto b : o.m_boxColliders)
            pObj->AddBoxCollider(b.m_center, b.m_extent, b.m_transform);

        AddObject(pObj);
    }
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

    fin >> Out->m_ID;

    fin >> buf;
    Out->m_tagResStatic = static_cast<TAG_RES_STATIC>(std::stoi(buf));

    fin >> Out->m_position.x >> Out->m_position.y >> Out->m_position.z
        >> Out->m_rotation.x >> Out->m_rotation.y >> Out->m_rotation.z
        >> Out->m_scale   .x >> Out->m_scale   .y >> Out->m_scale   .z;

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
    if (buf != "[")
    {
        return E_FAIL;
    }

    fin >> Out->m_center.x >> Out->m_center.y >> Out->m_center.z
        >> Out->m_extent.x >> Out->m_extent.y >> Out->m_extent.z
        >> Out->m_transform._11 >> Out->m_transform._12
        >> Out->m_transform._13 >> Out->m_transform._14
        >> Out->m_transform._21 >> Out->m_transform._22
        >> Out->m_transform._23 >> Out->m_transform._24
        >> Out->m_transform._31 >> Out->m_transform._32
        >> Out->m_transform._33 >> Out->m_transform._34
        >> Out->m_transform._41 >> Out->m_transform._42
        >> Out->m_transform._43 >> Out->m_transform._44;

    fin >> buf;
    if (buf != "]")
    {
        return E_FAIL;
    }

    return S_OK;
}
