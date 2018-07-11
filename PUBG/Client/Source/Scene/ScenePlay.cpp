#include "stdafx.h"
#include "ScenePlay.h"
#include "DirectionalLight.h"
#include "TerrainFeature.h"
#include "SkySphere.h"
#include "Grid.h"
#include "Item.h"
#include "HeightMap.h"
#include "ComponentTransform.h"
//#include "UIImage.h"

void ScenePlay::setAloneMode()
{
    Communication()()->m_myInfo.ID = 0;

    // 
    Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(300.0f, 100.0f, 300.0f);
    // ..

    const int myID = Communication()()->m_myInfo.ID;
    pPlayer = new Character(myID);
    characters.emplace_back(pPlayer);
    AddObject(pPlayer);
    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        if (i == myID) continue;

        Character* pOther = new Character(i);
        others.emplace_back(pOther);
        characters.emplace_back(pOther);
        AddObject(pOther);
    }

    TerrainFeature* tf = new TerrainFeature(TAG_RES_STATIC::Rock_1, D3DXVECTOR3(300.0f, 100.0f, 2000.0f), Vector3::UP, Vector3::ONE * 0.7f);
    D3DXMATRIX m;
    D3DXQUATERNION qR;
    D3DXQuaternionRotationAxis(&qR, &Vector3::UP, 1.0f);
    D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 300.0f), nullptr, &qR, &D3DXVECTOR3(300.0f, 100.0f, 2000.0f));
    tf->AddBoundingBox(m);
    AddObject(tf);

    tf = new TerrainFeature(TAG_RES_STATIC::Rock_1, D3DXVECTOR3(300.0f, 100.0f, 1000.0f), Vector3::UP, Vector3::ONE * 0.7f);
    D3DXQuaternionRotationAxis(&qR, &Vector3::UP, 1.0f);
    D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 300.0f), nullptr, &qR, &D3DXVECTOR3(300.0f, 100.0f, 1000.0f));
    tf->AddBoundingBox(m);
    AddObject(tf);

    //For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    //item = new Item(TAG_RES_STATIC::Head_Lv1, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);
    //
    //p = D3DXVECTOR3(20, 0, 10);
    //item = new Item(TAG_RES_STATIC::Armor_Lv1, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(30, 0, 10);
    //item = new Item(TAG_RES_STATIC::Back_Lv1, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(40, 0, 10);
    //item = new Item(TAG_RES_STATIC::Bandage, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(40, 0, 10);
    //item = new Item(TAG_RES_STATIC::Bandage, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(50, 0, 10);
    //item = new Item(TAG_RES_STATIC::FirstAidKit, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(50, 0, 10);
    //item = new Item(TAG_RES_STATIC::FirstAidKit, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(60, 0, 10);
    //item = new Item(TAG_RES_STATIC::MedKit, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(70, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(70, 0, 30);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(80, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(80, 0, 10);
    //item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(90, 0, 10);
    item = new Item(TAG_RES_STATIC::QBZ, p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(100, 0, 10);
    //item = new Item(TAG_RES_STATIC::Kar98k, p, r, s);
    //AddObject(item);
    //pPlayer->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(110, 0, 10);
    //item = new Item(TAG_RES_STATIC::RedDot, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(120, 0, 10);
    //item = new Item(TAG_RES_STATIC::Aimpoint2X, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //p = D3DXVECTOR3(130, 0, 10);
    //item = new Item(TAG_RES_STATIC::ACOG, p, r, s);
    //AddObject(item);
    //character0->PutItemInTotalInventory(item);

    //character0->ShowTotalInventory();
}

void ScenePlay::setWithOthersMode()
{
    const int myID = Communication()()->m_myInfo.ID;
    pPlayer = new Character(myID);
    characters.emplace_back(pPlayer);
    AddObject(pPlayer);
    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        if (i == myID) continue;

        Character* pOther = new Character(i);
        others.emplace_back(pOther);
        characters.emplace_back(pOther);
        AddObject(pOther);
    }

    AddObject(new SkySphere);

    TerrainFeature* tf = new TerrainFeature(TAG_RES_STATIC::Rock_1, D3DXVECTOR3(500.0f, 100.0f, 500.0f), Vector3::ZERO, Vector3::ONE);
    D3DXMATRIX m;
    D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 300.0f), nullptr, &Quaternion::IDENTITY, &D3DXVECTOR3(500.0f, 100.0f, 500.0f));
    tf->AddBoundingBox(m);
    AddObject(tf);

    ////For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    p = D3DXVECTOR3(70, 0, 10);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    for (int i = 0; i < 5; i++)
    {
        p = D3DXVECTOR3(70, 0, 10);
        item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
        AddObject(item);
        pPlayer->PutItemInTotalInventory(item);
    }

    p = D3DXVECTOR3(80, 0, 10);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(90, 0, 10);
    item = new Item(TAG_RES_STATIC::QBZ, p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(100, 0, 10);
    item = new Item(TAG_RES_STATIC::Kar98k, p, r, s);
    AddObject(item);
    pPlayer->PutItemInTotalInventory(item);


}

ScenePlay::ScenePlay()
    : IScene()
    , pPlayer(nullptr)
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::OnInit()
{
    // set mouse point to center
    POINT center;
    center.x = 1280 / 2;
    center.y = 720 / 2;
    ClientToScreen(g_hWnd, &center);
    SetCursorPos(center.x, center.y);

    SetDirectionalLight(new DirectionalLight);

    //AddObject(new SkySphere);
    AddObject(new Grid);

    SetHeightMap(new HeightMap);

    //cell space partitioning
    m_TotalCellSpaces.resize(CellSpace::DIMENSION * CellSpace::DIMENSION);

    //LoadObjectsFromFile("./Resource/save.txt");

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

    for (auto c : characters)
    {
        Debug << "Character " << c->GetIndex() << " hp : " << c->GetCharacterHealth() << '\n';
    }
}

const std::vector<Character*> ScenePlay::GetOthers() const
{
    return others;
}
