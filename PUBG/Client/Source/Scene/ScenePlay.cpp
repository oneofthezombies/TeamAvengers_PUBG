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


void ScenePlay::setAloneMode()
{
    Communication()()->m_myInfo.ID = 0;
    const int myID = Communication()()->m_myInfo.ID;
    Character* pPlayer = GetPlayer();
    Light()()->SetTarget(pPlayer->GetTransform());

    //old map
    Communication()()->m_roomInfo.playerInfos[0].position = D3DXVECTOR3(1900.0f, 200.0f, 1900.0f);
    Communication()()->m_roomInfo.playerInfos[0].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[0].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(100.0f, 200.0f, 2000.0f);
    Communication()()->m_roomInfo.playerInfos[1].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[1].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[2].position = D3DXVECTOR3(2000.0f, 200.0f, 100.0f);
    Communication()()->m_roomInfo.playerInfos[2].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[2].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[3].position = D3DXVECTOR3(2000.0f, 200.0f, 2000.0f);
    Communication()()->m_roomInfo.playerInfos[3].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[3].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    
    ////new map
    //Communication()()->m_roomInfo.playerInfos[0].position = D3DXVECTOR3(7000.0f, 5000.0f, 7000.0f);
    //Communication()()->m_roomInfo.playerInfos[0].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[0].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(5000.0f, 5000.0f, 18000.0f);
    //Communication()()->m_roomInfo.playerInfos[1].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[1].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Communication()()->m_roomInfo.playerInfos[2].position = D3DXVECTOR3(20000.0f, 5000.0f, 6000.0f);
    //Communication()()->m_roomInfo.playerInfos[2].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[2].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Communication()()->m_roomInfo.playerInfos[3].position = D3DXVECTOR3(19000.0f, 5000.0f, 19000.0f);
    //Communication()()->m_roomInfo.playerInfos[3].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[3].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

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
}

void ScenePlay::setWithOthersMode()
{
    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-500.0f, 1000.0f, -500.0f));
    Character* pPlayer = GetPlayer();
    Light()()->SetTarget(pPlayer->GetTransform());

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

    //Communication()()->m_roomInfo.playerInfos[0].position = D3DXVECTOR3(200.0f, 200.0f, 200.0f);
    //Communication()()->m_roomInfo.playerInfos[0].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[0].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(200.0f, 200.0f, 4848.0f);
    //Communication()()->m_roomInfo.playerInfos[1].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[1].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Communication()()->m_roomInfo.playerInfos[2].position = D3DXVECTOR3(4848.0f, 200.0f, 200.0f);
    //Communication()()->m_roomInfo.playerInfos[2].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[2].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Communication()()->m_roomInfo.playerInfos[3].position = D3DXVECTOR3(4848.0f, 200.0f, 4848.0f);
    //Communication()()->m_roomInfo.playerInfos[3].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[3].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    AddObject(new SkySphere);

    //TerrainFeature* tf = new TerrainFeature(TAG_RES_STATIC::Rock_1, D3DXVECTOR3(500.0f, 100.0f, 500.0f), Vector3::ZERO, Vector3::ONE);
    //D3DXMATRIX m;
    //D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 300.0f), nullptr, &Quaternion::IDENTITY, &D3DXVECTOR3(500.0f, 100.0f, 500.0f));
    //tf->AddBoundingBox(m);
    //AddObject(tf);

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

    //AddObject(new SkySphere);
    //AddObject(new Grid);

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

    AddCharacters();

    // No id received
    if (Communication()()->m_myInfo.ID == -1)
    {
        setAloneMode();
    }

    // Yes id received
    else if (Communication()()->m_myInfo.ID > -1)
    {
        setWithOthersMode();
    }

    Sound()()->Stop(0);
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

    Shader()()->AddShadowSource(Matrix::IDENTITY, pHeightMap->GetMesh(), 0);

    for (auto c : GetCharacters())
    {
        Debug << "Character " << c->GetIndex() << " hp : " << c->GetCharacterHealth() << '\n';
    }
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
