#include "stdafx.h"
#include "ScenePlay.h"
#include "DirectionalLight.h"
#include "TerrainFeature.h"
#include "SkySphere.h"
#include "Grid.h"
#include "Item.h"
#include "HeightMap.h"
#include "CollisionTestBox.h"

ScenePlay::ScenePlay()
    : IScene()
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::OnInit()
{
    //Collision()()->SubscribeCollisionEvent(TAG_COLLISION::Impassable, TAG_COLLISION::Player_0_Body);

    SetDirectionalLight(new DirectionalLight);
    
    //AddObject(new TerrainFeature(TAG_RES_STATIC::SkySphere, Vector3::ZERO, Vector3::ZERO, Vector3::ONE* 0.18f));

    //AddObject(new SkySphere);
    AddObject(new Grid);

    SetHeightMap(new HeightMap);
    //

    Communication()()->m_MyInfo.m_ID = 0;
    ////for (int i = 0; i < Character::NUM_PLAYER; ++i)
    ////    AddObject(new Character(i));
    Character* character0 = new Character(0);
    AddObject(character0);
    //AddObject(new Character(0));

    ////LoadObjectsFromFile("./Resource/save.txt");

    //TerrainFeature* pTerrainFeature = new TerrainFeature(TAG_RES_STATIC::Bandage, D3DXVECTOR3(0, 0, 0), Vector3::ONE, Vector3::ONE);
    //D3DXMATRIX stt, ttt;
    //D3DXMatrixScaling(&stt, 1.0f, 1.0f, 1.0f);
    ////D3DXMatrixTranslation(&ttt, 100.0f, 200.0f, 200.0f);
    //pTerrainFeature->AddBoxCollider(stt /** ttt*/);
    //AddObject(pTerrainFeature);

    //AddObject(new SampleCollisionBox);

    ////For inventory Test
    Item* item = nullptr;
    D3DXVECTOR3 p(10, 0, 10);
    D3DXVECTOR3 r(0, 0, 0);
    D3DXVECTOR3 s(1, 1, 1);

    ////item = new Item(TAG_RES_STATIC::Head_Lv1, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);
    ////
    ////p = D3DXVECTOR3(20, 0, 10);
    ////item = new Item(TAG_RES_STATIC::Armor_Lv1, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(30, 0, 10);
    ////item = new Item(TAG_RES_STATIC::Back_Lv1, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(40, 0, 10);
    ////item = new Item(TAG_RES_STATIC::Bandage, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(40, 0, 10);
    ////item = new Item(TAG_RES_STATIC::Bandage, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(50, 0, 10);
    ////item = new Item(TAG_RES_STATIC::FirstAidKit, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(50, 0, 10);
    ////item = new Item(TAG_RES_STATIC::FirstAidKit, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(60, 0, 10);
    ////item = new Item(TAG_RES_STATIC::MedKit, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(70, 0, 10);
    ////item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(70, 0, 10);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    AddObject(item);
    character0->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(70, 0, 10);
    item = new Item(TAG_RES_STATIC::Ammo_5_56mm, p, r, s);
    AddObject(item);
    character0->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(80, 0, 10);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    AddObject(item);
    character0->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(80, 0, 10);
    item = new Item(TAG_RES_STATIC::Ammo_7_62mm, p, r, s);
    AddObject(item);
    character0->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(90, 0, 10);
    item = new Item(TAG_RES_STATIC::QBZ, p, r, s);
    AddObject(item);
    character0->PutItemInTotalInventory(item);

    p = D3DXVECTOR3(100, 0, 10);
    item = new Item(TAG_RES_STATIC::Kar98k, p, r, s);
    AddObject(item);
    character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(110, 0, 10);
    ////item = new Item(TAG_RES_STATIC::RedDot, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(120, 0, 10);
    ////item = new Item(TAG_RES_STATIC::Aimpoint2X, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    ////p = D3DXVECTOR3(130, 0, 10);
    ////item = new Item(TAG_RES_STATIC::ACOG, p, r, s);
    ////AddObject(item);
    ////character0->PutItemInTotalInventory(item);

    //character0->ShowTotalInventory();
    ////LoadObjectsFromFile("./Resource/save.txt");
}

void ScenePlay::OnUpdate()
{

}
