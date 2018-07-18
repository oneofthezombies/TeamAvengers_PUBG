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

void ScenePlay::setAloneMode()
{
    Communication()()->m_myInfo.ID = 0;
    const int myID = Communication()()->m_myInfo.ID;
    pPlayer = new Character(myID);
    characters.emplace_back(pPlayer);
    AddObject(pPlayer);

    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-500.0f, 1000.0f, -500.0f));
    Light()()->SetTarget(pPlayer->GetTransform());

    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        if (i == myID) continue;

        Character* pOther = new Character(i);
        others.emplace_back(pOther);
        characters.emplace_back(pOther);
        AddObject(pOther);
    }

    Communication()()->m_roomInfo.playerInfos[0].position = D3DXVECTOR3(200.0f, 200.0f, 200.0f);
    Communication()()->m_roomInfo.playerInfos[0].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[0].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(200.0f, 200.0f, 308.0f);
    Communication()()->m_roomInfo.playerInfos[1].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[1].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[2].position = D3DXVECTOR3(4848.0f, 200.0f, 200.0f);
    Communication()()->m_roomInfo.playerInfos[2].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[2].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[3].position = D3DXVECTOR3(4848.0f, 200.0f, 4848.0f);
    Communication()()->m_roomInfo.playerInfos[3].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[3].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //Character* pOther = new Character(1);
    //others.emplace_back(pOther);
    //characters.emplace_back(pOther);
    //AddObject(pOther);
    //Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(600.0f, 100.0f, 600.0f);
    //Communication()()->m_roomInfo.playerInfos[1].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    //Communication()()->m_roomInfo.playerInfos[1].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //AddObject(new SkySphere);

    //TerrainFeature* tf = new TerrainFeature(TAG_RES_STATIC::Rock_1, D3DXVECTOR3(400.0f, 100.0f, 0.0f), Vector3::UP, Vector3::ONE * 0.7f);
    //D3DXMATRIX m;
    //D3DXQUATERNION qR;
    //D3DXQuaternionRotationAxis(&qR, &Vector3::UP, 1.0f);
    //D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 300.0f), nullptr, &qR, &D3DXVECTOR3(300.0f, 100.0f, 2000.0f));
    //tf->AddBoundingBox(m);
    //AddObject(tf);

    //tf = new TerrainFeature(TAG_RES_STATIC::Rock_1, D3DXVECTOR3(300.0f, 100.0f, 1000.0f), Vector3::UP, Vector3::ONE * 0.7f);
    //D3DXQuaternionRotationAxis(&qR, &Vector3::UP, 1.0f);
    //D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 300.0f), nullptr, &qR, &D3DXVECTOR3(300.0f, 100.0f, 1000.0f));
    //tf->AddBoundingBox(m);
    //AddObject(tf);

    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    item = new Item(TAG_RES_STATIC::Head_Lv1, "Head_Lv1_0", p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);
    
    p = D3DXVECTOR3(20, 0, 10);
    item = new Item(TAG_RES_STATIC::Armor_Lv1, "Armor_Lv1_0", p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(30, 0, 10);
    item = new Item(TAG_RES_STATIC::Back_Lv1, "Armor_Lv1_0", p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(60, 0, 10);
    //item = new Item(TAG_RES_STATIC::MedKit, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(60, 0, 10);
    //item = new Item(TAG_RES_STATIC::MedKit, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(60, 0, 10);
    //item = new Item(TAG_RES_STATIC::MedKit, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(170, 200, 130);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, "Ammo_5_56mm_0", p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(170, 200, 130);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, "Ammo_5_56mm_1", p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(170, 200, 130);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, "Ammo_5_56mm_2", p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(90, 200, 10);
    item = new Item(TAG_RES_STATIC::QBZ, "QBZ_0", p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(70, 0, 30);
    //item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(80, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(80, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(90, 200, 10);
    //item = new Item(TAG_RES_STATIC::QBZ, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    //p = D3DXVECTOR3(190, 200, 220);
    //item = new Item(TAG_RES_STATIC::QBZ, p, r, s);
    //AddObject(item);
    //InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    ////pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(100, 0, 10);
    //item = new Item(TAG_RES_STATIC::Kar98k, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(110, 0, 10);
    //item = new Item(TAG_RES_STATIC::RedDot, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(120, 0, 10);
    //item = new Item(TAG_RES_STATIC::Aimpoint2X, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(130, 0, 10);
    //item = new Item(TAG_RES_STATIC::ACOG, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);
}

void ScenePlay::setWithOthersMode()
{
    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    const int myID = Communication()()->m_myInfo.ID;
    pPlayer = new Character(myID);
    characters.emplace_back(pPlayer);
    AddObject(pPlayer);

    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-500.0f, 1000.0f, -500.0f));
    Light()()->SetTarget(pPlayer->GetTransform());

    //p = D3DXVECTOR3(100, 0, 10);
    //item = new Item(TAG_RES_STATIC::Kar98k, "gun", p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(90, 0, 10);
    //item = new Item(TAG_RES_STATIC::QBZ, "gun", p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(20, 0, 10);
    //item = new Item(TAG_RES_STATIC::Armor_Lv1, "Armor_Lv1_0", p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(30, 0, 10);
    //item = new Item(TAG_RES_STATIC::Back_Lv1, "Back_Lv1_0", p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        if (i == myID) continue;

        Character* pOther = new Character(i);
        others.emplace_back(pOther);
        characters.emplace_back(pOther);
        AddObject(pOther);

        //p = D3DXVECTOR3(100, 0, 10);
        //item = new Item(TAG_RES_STATIC::Kar98k, "gun", p, r, s);
        //AddObject(item);
        //pOther->PutItemInTotalInventory(item);

        //p = D3DXVECTOR3(90, 0, 10);
        //item = new Item(TAG_RES_STATIC::QBZ, "gun", p, r, s);
        //AddObject(item);
        //pOther->PutItemInTotalInventory(item);

        //pOther->PutItemInTotalInventory(item);

        //p = D3DXVECTOR3(20, 0, 10);
        //item = new Item(TAG_RES_STATIC::Armor_Lv1, "Armor_Lv1_0", p, r, s);
        //AddObject(item);
        //pOther->PutItemInTotalInventory(item);

        //p = D3DXVECTOR3(30, 0, 10);
        //item = new Item(TAG_RES_STATIC::Back_Lv1, "Back_Lv1_0", p, r, s);
        //AddObject(item);
        //pOther->PutItemInTotalInventory(item);
    }

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

    p = D3DXVECTOR3(90, 0, 10);
    name = "QBZ " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::QBZ, name, p, r, s);
    AddObject(item);
    InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, GetCellIndex(p), item);

    p = D3DXVECTOR3(70, 0, 10);
    name = "Ammo_5_56mm " + std::to_string(0);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, name, p, r, s);
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



    //p = D3DXVECTOR3(70, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //for (int i = 0; i < 5; i++)
    //{
    //    p = D3DXVECTOR3(70, 0, 10);
    //    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    //    AddObject(item);
    //    pPlayer->PutItemInTotalInventory(item);
    //}

    //p = D3DXVECTOR3(80, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(90, 0, 10);
    //item = new Item(TAG_RES_STATIC::QBZ, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(100, 0, 10);
    //item = new Item(TAG_RES_STATIC::Kar98k, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);


}

ScenePlay::ScenePlay()
    : IScene()
    , pPlayer(nullptr)
    , m_layer(nullptr)
{
}

ScenePlay::~ScenePlay()
{
    UI()()->Destroy(m_layer);
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

    LoadObjectsFromFile("./Resource/save.txt");

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

    static int testi;

    if (Input()()->IsOnceKeyDown(VK_LEFT))
    {
        testi--;
        if (testi < 1)
        {
            testi = 1;
        }
        Sound()()->Play(static_cast<TAG_SOUND>(testi), Vector3::ZERO, 1.0f, FMOD_2D);
    }
    if (Input()()->IsOnceKeyDown(VK_RIGHT))
    {
        testi++;
        if (testi > static_cast<int>(TAG_SOUND::Background))
        {
            testi = 21;
        }
        Sound()()->Play(static_cast<TAG_SOUND>(testi), Vector3::ZERO, 1.0f, FMOD_2D);
    }

    Shader()()->AddShadowSource(Matrix::IDENTITY, pHeightMap->GetMesh(), 0);

    for (auto c : characters)
    {
        Debug << "Character " << c->GetIndex() << " hp : " << c->GetCharacterHealth() << '\n';
    }
}

const std::vector<Character*> ScenePlay::GetOthers() const
{
    return others;
}

int ScenePlay::GetSurvivors() const
{
    int survivalNum = 0;
    for (auto c : characters)
    {
        if (c->GetCharacterIsDead() == false)
            survivalNum++;
    }
    return survivalNum;
}

Character* ScenePlay::GetPlayer() const
{
    return pPlayer;
}

const std::vector<Character*>& ScenePlay::GetCharacters() const
{
    return characters;
}

UIObject* ScenePlay::GetLayer(int layerIndex) const
{
    switch (layerIndex)
    {
    case 1: return m_layer->GetChild(0);
    case 2: return m_layer->GetChild(1);
    case 3: return m_layer->GetChild(2);
    case 4: return m_layer->GetChild(3);
    }
}
