#include "stdafx.h"
#include "IScene.h"
#include "IObject.h"
#include "TerrainFeature.h"
#include "DirectionalLight.h"

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
    , m_pDirectionalLight(nullptr)
{
}

IScene::~IScene()
{
	for (auto o : m_objects)
        SAFE_DELETE(o);

    SAFE_DELETE(m_pDirectionalLight);
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

        TerrainFeature* pObj = new TerrainFeature(
            o.m_tagResStatic, 
            o.m_position, 
            o.m_rotation, 
            o.m_scale);

        for (auto b : o.m_boxColliders)
            pObj->AddBoxCollider(b.m_transform);

        AddObject(pObj);
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

void IScene::SetDirectionalLight(DirectionalLight* p)
{
    assert(p && "IScene::SetDirectionalLight(), directional light is null.");

    SAFE_DELETE(m_pDirectionalLight);
    m_pDirectionalLight = p;
}

DirectionalLight* IScene::GetDirectionalLight()
{
    return m_pDirectionalLight;
}