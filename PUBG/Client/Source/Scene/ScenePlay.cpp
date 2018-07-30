#include "stdafx.h"
#include "ScenePlay.h"
#include "Light.h"
#include "TerrainFeature.h"
#include "SkySphere.h"
#include "Grid.h"
#include "Item.h"
#include "HeightMap.h"
#include "ComponentTransform.h"
#include "UITest.h"
#include "DeathDropBox.h"
#include "UIImage.h"
#include "UITest.h"
#include "MagneticField.h"

void ScenePlay::setAloneMode()
{
    //// for surface
    //EffectMesh* pChurchMesh = Resource()()->GetEffectMesh(TAG_RES_STATIC::ContainerBox_A);
    //MeshHelper::GetSurfaces(
    //    pChurchMesh->m_pMesh, 
    //    Vector3::UP, 
    //    D3DX_PI / 6, 
    //    &m_verticesChurch);

    //// for ui test
    //m_pUITest = new UITest;
    //UI()()->RegisterUIObject(m_pUITest);
    
    //자기장 먼저 create
    MagneticField* pMF = new MagneticField();
    pMF->Init();
    AddObject(pMF);

    Character* pPlayer = GetPlayer();
    Light()()->SetTarget(pPlayer->GetTransform());
    //Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-1500.0f, 2300.0f, -1500.0f));
    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-1000.0f, 4000.0f, -1000.0f));

    auto addItem = [this](const TAG_RES_STATIC tag, const std::string& name, const float x, const float y, const float z) 
    {
        const D3DXVECTOR3 p(x, y, z);
        Item* pItem = new Item(tag, name, p, Vector3::ZERO, Vector3::ONE);
        AddObject(pItem);
        InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), pItem);
    };

    addItem(TAG_RES_STATIC::Head_Lv1, "Head_Lv1 0", 12954.0f, 5810.0f, 13873.0f);
    addItem(TAG_RES_STATIC::Armor_Lv1, "Armor_Lv1 0", 12564.0f, 5810.0f, 13461.0f);
    addItem(TAG_RES_STATIC::Back_Lv1, "Back_Lv1 0", 13410.0f, 5810.0f, 13465.0f);
    addItem(TAG_RES_STATIC::Ammo_7_62mm, "Ammo_7_62mm 0", 12835.0f, 5810.0f, 12258.0f);
    addItem(TAG_RES_STATIC::Ammo_7_62mm, "Ammo_7_62mm 1", 12779.0f, 5810.0f, 13103.0f);
    addItem(TAG_RES_STATIC::Kar98k, "Kar98k 0", 12694.0f, 5810.0f, 12901.0f);
    addItem(TAG_RES_STATIC::Ammo_5_56mm, "Ammo_5_56mm 0", 13129.0f, 5810.0f, 13312.0f);
    addItem(TAG_RES_STATIC::Ammo_5_56mm, "Ammo_5_56mm 1", 13135.0f, 5810.0f, 13079.0f);
    addItem(TAG_RES_STATIC::QBZ, "QBZ 0", 13318.0f, 5810.0f, 12861.0f);

    ////For inventory Test
    //Item* item = nullptr;
    //D3DXVECTOR3 p(Vector3::ZERO);
    //D3DXVECTOR3 r(Vector3::ZERO);
    //D3DXVECTOR3 s(Vector3::ONE);
    //std::string name;

    //p = D3DXVECTOR3(12954.0f, 5800.0f, 13873.0f);
    //name = "Head_Lv1 " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::Head_Lv1, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(12564.0f, 5800.0f, 13461.0f);
    //name = "Armor_Lv1 " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::Armor_Lv1, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(13410.0f, 5800.0f, 13465.0f);
    //name = "Back_Lv1 " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::Back_Lv1, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "QBZ " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "Ammo_5_56mm " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "QBZ " + std::to_string(1);
    //item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "Kar98k " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::Kar98k, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "Kar98k " + std::to_string(1);
    //item = new Item(TAG_RES_STATIC::Kar98k, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "Ammo_5_56mm " + std::to_string(1);
    //item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(1100.0f, 200.0f, 200.0f);
    //name = "Ammo_5_56mm " + std::to_string(2);
    //item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    //name = "Ammo_7_62mm " + std::to_string(0);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(1300.0f, 200.0f, 200.0f);
    //name = "Ammo_7_62mm " + std::to_string(1);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    // 빈 총알을 셋팅합니다. 이것은 총을 버릴 때 총에 들어있던 총알을 버리기 위해서 종종 필요합니다.
    setEmptyBullets();
}

void ScenePlay::setWithOthersMode()
{
    //자기장 먼저 create
    MagneticField* pMF = new MagneticField();
    pMF->Init();
    AddObject(pMF);


    Character* pPlayer = GetPlayer();
    Light()()->SetTarget(pPlayer->GetTransform());
    //Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-1500.0f, 2300.0f, -1500.0f));
    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-1000.0f, 2300.0f, -1000.0f));

    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    p = D3DXVECTOR3(200.0f, 200.0f, 200.0f);
    string name = "Head_Lv1 " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Head_Lv1, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(300.0f, 200.0f, 200.0f);
    name = "Armor_Lv1 " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Armor_Lv1, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(400.0f, 200.0f, 200.0f);
    name = "Back_Lv1 " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Back_Lv1, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(500.0f, 200.0f, 200.0f);
    name = "QBZ " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(600.0f, 200.0f, 200.0f);
    name = "Ammo_5_56mm " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(700.0f, 200.0f, 200.0f);
    name = "QBZ " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(800.0f, 200.0f, 200.0f);
    name = "Kar98k " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Kar98k, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(900.0f, 200.0f, 200.0f);
    name = "Kar98k " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::Kar98k, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(1000.0f, 200.0f, 200.0f);
    name = "Ammo_5_56mm " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(1100.0f, 200.0f, 200.0f);
    name = "Ammo_5_56mm " + std::to_string(2);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(1200.0f, 200.0f, 200.0f);
    name = "Ammo_7_62mm " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(1300.0f, 200.0f, 200.0f);
    name = "Ammo_7_62mm " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    // 빈 총알을 셋팅합니다. 이것은 총을 버릴 때 총에 들어있던 총알을 버리기 위해서 종종 필요합니다.
    setEmptyBullets();
}

void ScenePlay::setEmptyBullets()
{
    std::vector<Character*> characters = GetCharacters();

    const int numEmpty = 5;
    for (int ch_i = 0; ch_i < GameInfo::NUM_PLAYERS; ++ch_i)
    {
        for (int i = 0; i < numEmpty; ++i)
        {
            std::string name = "Ammo_5_56mm_for_empty_" + std::to_string(ch_i) + std::to_string(i);
            Item* item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, Vector3::ZERO, Vector3::ZERO, Vector3::ONE);
            item->SetCount(0);

            Character* pCharacter = nullptr;
            for (auto pC : characters)
            {
                if (pC->GetIndex() == ch_i)
                {
                    pCharacter = pC;
                    break;
                }
            }

            pCharacter->GetTotalInventory().m_empties[item->GetTagResStatic()].emplace_back(item);

            name = "Ammo_7_62mm_for_empty_" + std::to_string(ch_i) + std::to_string(i);
            item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, Vector3::ZERO, Vector3::ZERO, Vector3::ONE);
            item->SetCount(0);

            pCharacter->GetTotalInventory().m_empties[item->GetTagResStatic()].emplace_back(item);
        }
    }
}

ScenePlay::ScenePlay()
    : IScene(TAG_SCENE::Play)
    , m_layer(nullptr)
    , m_coolDown(0.0f)
    , m_coolTime(3.0f)
    , m_pMagneticField(nullptr)
    , pSplash(nullptr)
{
}

ScenePlay::~ScenePlay()
{
    UI()()->Destroy(m_layer);

    for (auto box : deathDropBoxes)
    {
        for (auto item : box->GetItems())
        {
            SAFE_DELETE(item);
        }
    }

    UI()()->Destroy(m_pUITest);
}

void ScenePlay::Render()
{
    for (auto i : firstGroup)
        if (i) i->Render();

    for (auto i : secondGroup)
        if (i) i->Render();

    std::map<float, IObject*> sortedByDistance;
    const D3DXVECTOR3 cameraPos = CurrentCamera()()->GetPosition();
    for (auto o : thirdGroup)
    {
        const D3DXVECTOR3 v = o->GetTransform()->GetPosition() - cameraPos;
        const float lenSq = D3DXVec3LengthSq(&v);
        sortedByDistance.emplace(lenSq, o);
    }
    
    Render(sortedByDistance);
    
    if (m_pMagneticField)
    {
        if (m_pMagneticField->IsInside(cameraPos))
        {
            Device()()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
            m_pMagneticField->Render();
            Device()()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        }
        else
        {
            m_pMagneticField->Render();
        }
    }
}

void ScenePlay::AddObject(IObject* p)
{
    IScene::AddObject(p);

    switch (p->GetTagObject())
    {
    case TAG_OBJECT::TerrainFeature:
        {
            TerrainFeature* pTerrainFeature = static_cast<TerrainFeature*>(p);
            const TAG_RES_STATIC tagResStatic = pTerrainFeature->GetTagResStatic();
            switch (tagResStatic)
            {
            case TAG_RES_STATIC::AlaskaCedar:
            case TAG_RES_STATIC::AmericanElem:
            case TAG_RES_STATIC::LondonPlane:
            case TAG_RES_STATIC::Grass_1:
            case TAG_RES_STATIC::Grass_2:
            case TAG_RES_STATIC::DeadGrass:
            case TAG_RES_STATIC::Dogwood:
                {
                    thirdGroup.emplace(p);
                }
                break;
            default:
                {
                    firstGroup.emplace(p);
                }
                break;
            }
        }
        break;
    case TAG_OBJECT::Water:
        {
            secondGroup.emplace(p);
        }
        break;
    case TAG_OBJECT::MagneticField:
        {
            SAFE_DELETE(m_pMagneticField);
            m_pMagneticField = static_cast<MagneticField*>(p);
        }
        break;
    default:
        {
            firstGroup.emplace(p);
        }
        break;
    }
}

void ScenePlay::OnInit()
{
    m_layer = new UIObject(nullptr);
    UI()()->RegisterUIObject(m_layer);

    new UIObject(m_layer);
    new UIObject(m_layer); 
    new UIObject(m_layer);
    new UIObject(m_layer);
    
    // set mouse point to center
    POINT center;
    center.x = 1280 / 2;
    center.y = 720 / 2;
    ClientToScreen(g_hWnd, &center);
    SetCursorPos(center.x, center.y);
    ShowCursor(false);

    AddObject(new SkySphere);

    SetHeightMap(new HeightMap);

    //cell space partitioning
    size_t totalSpaceNum = CellSpace::DIMENSION * CellSpace::DIMENSION;
    m_TotalCellSpaces.resize(totalSpaceNum);
    for (size_t i = 0; i < totalSpaceNum; i++)
    {
        m_TotalCellSpaces[i].pIndex = i;
    }
    
    LoadObjectsFromFile("./Resource/save.txt");
    //LoadObjectsFromFile("./Resource/save1.txt");

    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        DeathDropBox* pBox = new DeathDropBox;
        deathDropBoxes.emplace_back(pBox);
        AddObject(pBox);
    }   

    //for water
    pWater = new Water;
    pWater->Init(82712.3f * 2.0f, 82712.3f * 2.0f, 20.0f);
    AddObject(pWater);

    // No id received
    if (Communication()()->m_myInfo.ID == -1)
    {
        auto& pis = Communication()()->m_roomInfo.playerInfos;
        for (std::size_t i = 0; i < pis.size(); i++)
        {
            pis[i].ID = static_cast<int>(i);
        }

        Communication()()->m_myInfo.ID = 1;

        AddCharacters();
        setAloneMode();
    }

    // Yes id received
    else if (Communication()()->m_myInfo.ID > -1)
    {
        AddCharacters();
        setWithOthersMode();
    }

    Sound()()->Stop(0);

    pSplash = new UIImage("./Resource/", "LoadingScreen.tga", Vector3::ZERO, nullptr, m_layer);
    m_coolDown = m_coolTime;
}

void ScenePlay::OnUpdate()
{
    // 클라 캐릭이랑 다른사람꺼 총알이랑 충돌체크
    /*
    BOOL D3DXSphereBoundProbe(
  _In_ const D3DXVECTOR3 *pCenter,
  _In_       FLOAT       Radius,
  _In_ const D3DXVECTOR3 *pRayPosition,
  _In_ const D3DXVECTOR3 *pRayDirection
);
    탄도학
    검출 방법
    레이,
    */

    //static int testi;

    //if (Input()()->IsOnceKeyDown(VK_LEFT))
    //{
    //    testi--;
    //    if (testi < 1)
    //    {
    //        testi = 1;
    //    }
    //    Sound()()->Play(static_cast<TAG_SOUND>(testi), Vector3::ZERO, 1.0f, FMOD_2D);
    //}
    //if (Input()()->IsOnceKeyDown(VK_RIGHT))
    //{
    //    testi++;
    //    if (testi > static_cast<int>(TAG_SOUND::Background))
    //    {
    //        testi = 21;
    //    }
    //    Sound()()->Play(static_cast<TAG_SOUND>(testi), Vector3::ZERO, 1.0f, FMOD_2D);
    //}

    if (pSplash->IsRender())
    {
        const float dt = Time()()->GetDeltaTime();
        m_coolDown -= dt;
        if (m_coolDown <= 0.0f)
        {
            pSplash->SetIsRender(false);
        }
    }

    if (pHeightMap)
    {
        Shader()()->AddShadowSource(Matrix::IDENTITY, pHeightMap->GetMesh(), 0);
    }

    //for (auto c : GetCharacters())
    //{
    //    Debug << "Character " << c->GetIndex() << " hp : " << c->GetCharacterHealth() << '\n';
    //}
}

const std::vector<Character*>& ScenePlay::GetOthers() const
{
    return Scene()()->GetOthers();
}

int ScenePlay::GetSurvivors() const
{
    int survivalNum = 0;
    for (auto c : GetCharacters())
    {
        if (c->GetCharacterIsDead() == false)
            survivalNum++;
    }
    return survivalNum;
}

Character* ScenePlay::GetPlayer() const
{
    return Scene()()->GetPlayer();
}

const std::vector<Character*>& ScenePlay::GetCharacters() const
{
    return Scene()()->GetCharacters();
}

DeathDropBox* ScenePlay::GetDeathDropBox(const std::size_t index)
{
    assert(
        index < deathDropBoxes.size() && 
        deathDropBoxes[index] && 
        "ScenePlay::GetDeathDropBox()");

    return deathDropBoxes[index];
}

UIObject* ScenePlay::GetLayer(int layerIndex) const
{
    switch (layerIndex)
    {
    case 1: return m_layer->GetChild(0);
    case 2: return m_layer->GetChild(1);
    case 3: return m_layer->GetChild(2);
    case 4: return m_layer->GetChild(3);
    default:
        return m_layer->GetChild(0);
        assert(false && "ScenePlay::GetLayer(), default case.");
    }
}

void ScenePlay::AddCharacters()
{
    for (auto c : Scene()()->GetCharacters())
    {
        c->InitScenePlay();
        AddObject(c);
    }
}

void ScenePlay::SortByDistance(
    const D3DXVECTOR3& cameraPos,
    const std::set<IObject*>& objects,
    std::map<float, IObject*>* OutObjects)
{
    assert(OutObjects && "sortByDistance(), argument is null.");

    OutObjects->clear();

    for (auto o : objects)
    {
        if (!o) continue;

        const D3DXVECTOR3 v = o->GetTransform()->GetPosition() - cameraPos;
        const float lenSq = D3DXVec3LengthSq(&v);
        OutObjects->emplace(lenSq, o);
    }
}

void ScenePlay::Render(const std::map<float, IObject*>& sortedObjects)
{
    for (
        auto it = sortedObjects.rbegin();
        it != sortedObjects.rend();
        ++it)
    {
        IObject* pO = it->second;
        pO->Render();
    }
}
