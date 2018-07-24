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

void ScenePlay::setAloneMode()
{
    // follow light
    Character* pPlayer = GetPlayer();
    Light()()->SetTarget(pPlayer->GetTransform());
    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-1500.0f, 2300.0f, -1500.0f));

    //DeathDropBox* pLightTarget = new DeathDropBox;
    //pLightTarget->GetTransform()->SetPosition(D3DXVECTOR3(10000.0f, 0.0f, 10000.0f));
    //AddObject(pLightTarget);
    //Light()()->SetTarget(pLightTarget->GetTransform());
    //Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-20000.0f, 50000.0f, -20000.0f));
        
    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    string name = "Head_Lv1 " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Head_Lv1, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Armor_Lv1 " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Armor_Lv1, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Back_Lv1 " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Back_Lv1, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "QBZ " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Ammo_5_56mm " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "QBZ " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Kar98k " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Kar98k, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Kar98k " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::Kar98k, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Ammo_5_56mm " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(1100.0f, 200.0f, 200.0f);
    name = "Ammo_5_56mm " + std::to_string(2);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(7000.0f, 4700.0f, 7000.0f);
    name = "Ammo_7_62mm " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(1300.0f, 200.0f, 200.0f);
    name = "Ammo_7_62mm " + std::to_string(1);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    AddObject(new SkySphere);

    // 빈 총알을 셋팅합니다. 이것은 총을 버릴 때 총에 들어있던 총알을 버리기 위해서 종종 필요합니다.
    setEmptyBullets();
}

void ScenePlay::setWithOthersMode()
{
    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    Character* pPlayer = GetPlayer();
    Light()()->SetTarget(pPlayer->GetTransform());
    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-1500.0f, 2300.0f, -1500.0f));

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

    AddObject(new SkySphere);

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

    SetHeightMap(new HeightMap);

    //cell space partitioning
    size_t totalSpaceNum = CellSpace::DIMENSION * CellSpace::DIMENSION;
    m_TotalCellSpaces.resize(totalSpaceNum);
    for (size_t i = 0; i < totalSpaceNum; i++)
    {
        m_TotalCellSpaces[i].pIndex = i;
    }
    
    //LoadObjectsFromFile("./Resource/save.txt");
    //LoadObjectsFromFile("./Resource/save1.txt");

    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        DeathDropBox* pBox = new DeathDropBox;
        deathDropBoxes.emplace_back(pBox);
        AddObject(pBox);
    }

    // No id received
    if (Communication()()->m_myInfo.ID == -1)
    {
        auto& pis = Communication()()->m_roomInfo.playerInfos;
        for (std::size_t i = 0; i < pis.size(); i++)
        {
            pis[i].ID = i;
        }

        Communication()()->m_myInfo.ID = 0;

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
