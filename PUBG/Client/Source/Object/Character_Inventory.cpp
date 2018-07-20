#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
///////추가한거 UI
#include "UIImage.h"
#include "UIText.h"
#include "TagUIPosition.h"
#include "UIButton.h"
#include "ScenePlay.h"

using Event       = UIButtonWithItem::Event;
using MouseButton = UIButtonWithItem::MouseButton;

const float Character::TotalInventory::DEFAULT_CAPACITY = 70.0f;
const float Character::TotalInventory::FIRST_LINE = 233.0f;
const float Character::TotalInventory::SECOND_LINE = 500.0f;

Character::TotalInventory::TotalInventory()
    : m_pHand(nullptr)
    , m_pEquipArmor(nullptr)
    , m_pEquipBack(nullptr)
    , m_pEquipHead(nullptr)
    , m_equipOnNum(0)

    , m_pWeaponPrimary(nullptr)
    , m_pWeaponSecondary(nullptr)
    , m_capacity(DEFAULT_CAPACITY)
    , m_numReload(0)
    , m_bulletFireCoolDown(0)

    , pTempSaveWeaponForX(nullptr)
    , pCharacter(nullptr)
    , pUIPicked(nullptr)
    , m_pWeapon1(nullptr)
    , m_pWeapon2(nullptr)
    , m_pUIArmor(nullptr)
    , m_pUIHead(nullptr)
    , m_pUIBack(nullptr)
    , m_stateClicked(false)
    , m_handState(TAG_RIFLE::None)

    , isOpened(false)
{
}

Character::TotalInventory::~TotalInventory()
{
    SAFE_DELETE(m_pHand);

    for (auto v : m_mapInventory)
        for (auto i : v.second)
            SAFE_DELETE(i);

    SAFE_DELETE(m_pEquipArmor);
    SAFE_DELETE(m_pEquipBack);
    SAFE_DELETE(m_pEquipHead);
              
    SAFE_DELETE(m_pWeaponPrimary);
    SAFE_DELETE(m_pWeaponSecondary);
}

void Character::TotalInventory::Init()
{
    ScenePlay* scenePlay = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    UIObject* layer1 = scenePlay->GetLayer(1);

    // 검정 배경
    pBorder = new UIImage(
        "./Resource/UI/Inventory/Basic/",
        "black_1280_720_70.png",
        Vector3::ZERO,
        nullptr,
        layer1);

    //캐릭터 닉네임 텍스트
    string nickName = Communication()()->m_myInfo.nickname;
    auto nickNameText = new UIText(
        Resource()()->GetFont(TAG_FONT::Inventory_NickName),
        D3DXVECTOR2(120.0f, 20.0f),
        nickName,
        D3DCOLOR_XRGB(255, 255, 255),
        pBorder);
    nickNameText->SetDrawTextFormat(DT_CENTER);
    nickNameText->SetPosition(D3DXVECTOR3(590.0f, 22.0f, 0.0f));


    // 텍스트
    auto a = new UIText(
        Resource()()->GetFont(TAG_FONT::Inventory_28),
        D3DXVECTOR2(100.0f, 20.0f),
        string("VICINITY"),
        D3DCOLOR_XRGB(200, 200, 200),
        pBorder);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(74.0f, 24.0f, 0.0f));

    // 텍스트
    a = new UIText(
        Resource()()->GetFont(TAG_FONT::Inventory_Ground),
        D3DXVECTOR2(100.0f, 20.0f),
        string("Ground"),
        D3DCOLOR_XRGB(200, 200, 200),
        pBorder);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(74.0f, 77.0f, 0.0f));

    // 텍스트 
    a = new UIText(
        Resource()()->GetFont(TAG_FONT::Inventory_Ground),
        D3DXVECTOR2(100.0f, 20.0f),
        string("Sortby Type"),
        D3DCOLOR_XRGB(200, 200, 200),
        pBorder);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(264.0f, 77.0f, 0.0f));

    // 드롭드 인벤 구분선
    new UIImage(
        "./Resource/UI/Inventory/Basic/",
        "line.png",
        D3DXVECTOR3(233.0f, 92.0f, 0.0f),
        nullptr,
        pBorder);

    // 여자 캐릭터
    auto b = new UIImage(
        "./Resource/UI/Inventory/Character/",
        "Female.png",
        D3DXVECTOR3(410.0f, 85.0f, 0.0f),
        nullptr,
        pBorder);

    SetEquipUI();

    pBorder->SetIsRender(false);
}

void Character::TotalInventory::Destroy()
{
    UI()()->Destroy(m_pWeapon1);
    UI()()->Destroy(m_pWeapon2);
}

void Character::TotalInventory::Open()
{
    isOpened = true;
    // move ui to in screen

    pBorder->SetIsRender(isOpened);
}

void Character::TotalInventory::Close()
{
    isOpened = false;
    // move ui to out screen
    pBorder->SetIsRender(isOpened);

    POINT center;
    center.x = 1280 / 2;
    center.y = 720 / 2;
    ClientToScreen(g_hWnd, &center);
    SetCursorPos(center.x, center.y);
}

void Character::TotalInventory::Update()
{
    if (isOpened)
    {
        const D3DXVECTOR3 myPos = pCharacter->GetTransform()->GetPosition();

        // set ui dropped
        std::multimap<float, Item*> sortedItems;
        for (std::size_t i = 0; i < droppedItems.size(); ++i)
        {
            Item* pItem = droppedItems[i];
            const D3DXVECTOR3 itemPos = pItem->GetTransform()->GetPosition();
            const float dist = D3DXVec3Length(&(itemPos - myPos));
            sortedItems.emplace(dist, pItem);
        }

        for (auto u : m_uiDroped)
        {
            u->SetIsActive(false);
            u->pItem = nullptr;
        }
        droppedItems.clear();
        for (auto it = sortedItems.begin(); it != sortedItems.end(); ++it)
        {
            const std::size_t idx = std::distance(sortedItems.begin(), it);
            if (idx >= TotalInventory::NUM_UI_DROPPED) break;

            Item* pItem = it->second;
            pItem->SetIsRenderUIImage(true);

            UIButtonWithItem* pUI = m_uiDroped[idx];
            pUI->pItem = pItem;
            pUI->m_tagUIPosition = UIPosition::GetTag(TAG_UI_POSITION::dropped_0, idx);
            pUI->pUIImage = pItem->GetUIImage();
            //pUI->SetText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
            //    string(ItemInfo::GetName(pItem->GetTagResStatic()) +"   "+ to_string(pItem->GetCount())),
            //    D3DCOLOR_XRGB(255, 255, 255));
            pItem->GetUIText()->SetText(string(ItemInfo::GetName(pItem->GetTagResStatic()) + "   " + to_string(pItem->GetCount())));
            pUI->SetIsActive(true);
        }
        // end set ui dropped

        // set ui inven
        for (auto u : m_uiInven)
        {
            u->SetIsActive(false);
            u->pItem = nullptr;
        }
        std::size_t idx = 0;
        for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); ++it)
        {
            if (idx >= TotalInventory::NUM_UI_DROPPED) break;

            const std::vector<Item*>& items = it->second;
            for (auto item : items)
            {
                item->SetIsRenderUIImage(true);

                UIButtonWithItem* pUI = m_uiInven[idx];
                pUI->pItem = item;
                pUI->m_tagUIPosition = UIPosition::GetTag(TAG_UI_POSITION::inven_0, idx);
                pUI->pUIImage = item->GetUIImage();
                item->GetUIText()->SetText(string(ItemInfo::GetName(item->GetTagResStatic()) + "   " + to_string(item->GetCount())));
                pUI->SetIsActive(true);
                idx++;
            }
        }
        // end set ui inven
    }
}

void Character::TotalInventory::Render()
{
    if (isOpened)
    {
        // draw
    }
}

bool Character::TotalInventory::IsOpened()
{
    return isOpened;
}

void Character::TotalInventory::SetEquipUI()
{
    // 드롭드 칸 이미지
    int left = 74;
    int top = 92;
    const int height = 40;
    const int gap = 3;
    m_uiDroped.resize(NUM_UI_DROPPED);
    for (std::size_t i = 0; i < m_uiDroped.size(); ++i)
    {
        auto& u = m_uiDroped[i];

        u = new UIButtonWithItem(
            D3DXVECTOR3(
                static_cast<float>(left), 
                static_cast<float>(top + i * (height + gap)), 0.0f),
            "./Resource/UI/Inventory/Basic/",
            "ItemSlot.png",
            "ItemSlot_mouseover.png",
            "ItemSlot.png",
            pBorder,
            Resource()()->GetFont(TAG_FONT::Inventory_Ground),
            "",
            D3DCOLOR_XRGB(255, 255, 255),
            nullptr,
            TAG_UI_POSITION::dropped_0,
            std::bind(
                &Character::onMouse,
                pCharacter,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3));

        u->SetIsActive(false);
    }

    // 픽된 아이템 이미지
    ScenePlay* scenePlay = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    UIObject* layer3 = scenePlay->GetLayer(3);

    const float max = std::numeric_limits<float>::max();
    pUIPicked = new UIButtonWithItem(
        Vector3::ONE * max,
        "./Resource/UI/Inventory/Basic/",
        "Equip_click.png",
        "Equip_click.png",
        "Equip_click.png",
        layer3, nullptr, "", 0, nullptr,
        TAG_UI_POSITION::picked,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));

    new UIImage("", "", Vector3::ZERO, nullptr, pUIPicked);

    // 인벤토리 칸 이미지
    left = 243;
    top = 93;
    m_uiInven.resize(NUM_UI_DROPPED);
    for (std::size_t i = 0; i < m_uiInven.size(); ++i)
    {
        auto& u = m_uiInven[i];
        D3DXVECTOR3 pos = D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top + i * (height + gap)), 0.0f);

        u = new UIButtonWithItem(
            pos,
            "./Resource/UI/Inventory/Basic/",
            "ItemSlot.png",
            "ItemSlot_mouseover.png",
            "ItemSlot.png",
            pBorder,
            Resource()()->GetFont(TAG_FONT::Inventory_Ground),
            "",
            D3DCOLOR_XRGB(255, 255, 255),
            nullptr,
            TAG_UI_POSITION::dropped_0,
            std::bind(
                &Character::onMouse,
                pCharacter,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3));

        u->SetIsActive(false);
    }


    //헬멧
    left = 440;
    top = 93;
    m_pUIHead = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        pBorder,
        nullptr,"",0,nullptr,
        TAG_UI_POSITION::Helmat,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    //u->SetIsActive(false);

    //가방
    left = 440;
    top = 248;
    m_pUIBack = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        pBorder,
         nullptr, "", 0, nullptr,
        TAG_UI_POSITION::Bag,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    //u->SetIsActive(false);

     //아머
    left = 440;
    top = 293;
    m_pUIArmor = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        pBorder,
         nullptr, "", 0, nullptr,
        TAG_UI_POSITION::Armor,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    //u->SetIsActive(false);

     //벨트
    left = 440;
    top = 337;
    auto u = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        pBorder,
         nullptr, "", 0, nullptr,
        TAG_UI_POSITION::Belt,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    //u->SetIsActive(false);


     //무기 슬롯 1
    left = 870;
    top = 93;
    new UIImage("./Resource/UI/Inventory/Basic/",
        "WeaponBox_idle.png", D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        nullptr, pBorder);
    m_pWeapon1 = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "WeaponBox_idle.png",
        "WeaponBox_mouseover.png",
        "WeaponBox_possible.png",
        pBorder,
        Resource()()->GetFont(TAG_FONT::Inventory_Ground),
        "",
        D3DCOLOR_XRGB(255, 255, 255),
        nullptr,
        TAG_UI_POSITION::Weapon1,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
        m_pWeapon1->SetIsActive(false);


     //무기 슬롯 2
    left = 870;
    top = 234;

    new UIImage("./Resource/UI/Inventory/Basic/",
        "WeaponBox_idle.png", D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        nullptr, pBorder);

    
        m_pWeapon2 = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(left),
             static_cast<float>(top), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "WeaponBox_idle.png",
         "WeaponBox_mouseover.png",
         "WeaponBox_possible.png",
         pBorder,
         Resource()()->GetFont(TAG_FONT::Inventory_Ground),
         "",
         D3DCOLOR_XRGB(255, 255, 255),
         nullptr,
         TAG_UI_POSITION::Weapon2,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
        m_pWeapon2->SetIsActive(false);
     //gunu2->SetTexture

     //무기 슬롯 3
     left = 870;
     top = 375;
     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(left),
             static_cast<float>(top), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "WeaponBox_idle.png",
         "WeaponBox_mouseover.png",
         "WeaponBox_possible.png",
         pBorder,
         Resource()()->GetFont(TAG_FONT::Inventory_Ground),
         "",
         D3DCOLOR_XRGB(255, 255, 255),
         nullptr,
         TAG_UI_POSITION::Weapon3,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);

     //weapon slot 1
     auto imageu =new UIImage("./Resource/UI/Inventory/Basic/", "WeaponBoxNum.png", 
         D3DXVECTOR3(5, 6, 0), nullptr, m_pWeapon1);
     new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
         D3DXVECTOR2(20, 20), "1", D3DCOLOR_XRGB(255, 255, 255), imageu);

     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(187),
             static_cast<float>(15), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon1,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);

     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(6),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon1,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);


     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(73),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon1,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);


     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(147),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon1,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);

     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(288),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon1,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);

     //weapon slot 2
     imageu = new UIImage("./Resource/UI/Inventory/Basic/", "WeaponBoxNum.png",
         D3DXVECTOR3(5, 6, 0), nullptr, m_pWeapon2);
     new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
         D3DXVECTOR2(20, 20), "2", D3DCOLOR_XRGB(255, 255, 255), imageu);

     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(187),
             static_cast<float>(15), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon2,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);

     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(6),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon2,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);


     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(73),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon2,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);


     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(147),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon2,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);

     u = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(288),
             static_cast<float>(102), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "Attach_no.png",
         "Attach_yes.png",
         "Attach_click.png",
         m_pWeapon2,
         nullptr, "", 0, nullptr,
         TAG_UI_POSITION::Helmat,
         std::bind(
             &Character::onMouse,
             pCharacter,
             std::placeholders::_1,
             std::placeholders::_2,
             std::placeholders::_3));
     //u->SetIsActive(false);
}

void Character::TotalInventory::DropItem(Item** ppOriginItem)
{
    Item* originItem = *ppOriginItem;

    // 용량을 줄인다
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(originItem->GetTagResStatic());
    m_capacity -= ItemInfo::GetCapacityExtension(originItem->GetTagResStatic());
    if (category == TAG_ITEM_CATEGORY::Armor ||
        category == TAG_ITEM_CATEGORY::Back ||
        category == TAG_ITEM_CATEGORY::Head)
        m_equipOnNum--;

    // 아이템을 필드로
    originItem->SetIsInInventory(false);
    originItem->SetIsRenderEffectMesh(true);
    originItem->SetIsRenderSkinnedMesh(false);
    originItem->GetTransform()->SetRotation(Vector3::ZERO);
    originItem->GetTransform()->Update();

    CurrentScene()()->AddObject(originItem);
    const std::size_t cellIndex = CurrentScene()()->GetCellIndex(originItem->GetTransform()->GetPosition());
    CurrentScene()()->InsertObjIntoTotalCellSpace(originItem->GetTagObject(), cellIndex, originItem);

    *ppOriginItem = nullptr;
}

int Character::TotalInventory::EquipItem(Item** ppOriginItem, Item* pNewItem)
{
    // 용량을 늘린다
    m_capacity += ItemInfo::GetCapacityExtension(pNewItem->GetTagResStatic());
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(pNewItem->GetTagResStatic());
    if (category == TAG_ITEM_CATEGORY::Armor ||
        category == TAG_ITEM_CATEGORY::Back ||
        category == TAG_ITEM_CATEGORY::Head)
        m_equipOnNum++;

    // 아이템을 낀다
    pNewItem->SetIsInInventory(true);
    pNewItem->SetIsRenderEffectMesh(false);
    pNewItem->SetIsRenderSkinnedMesh(true);

    *ppOriginItem = pNewItem;

    if (pNewItem->IsInDeathDropBox())
    {
        const int boxID = pNewItem->GetDeathDropBoxIndex();
        pNewItem->DeleteThisInDeathDropBox();

        return boxID;
    }
    else // in field
    {
        const std::size_t cellIndex = CurrentScene()()->GetCellIndex(pNewItem->GetTransform()->GetPosition());
        CurrentScene()()->ItemIntoInventory(cellIndex, pNewItem);
        CurrentScene()()->RemoveObject(pNewItem);

        return -1;
    }
}

void Character::TotalInventory::DropPrimary()
{
    if (!m_pWeaponPrimary) return;

    const std::string originItemName = m_pWeaponPrimary->GetName();

    ReleaseBullets(m_pWeaponPrimary);
    DropItem(&m_pWeaponPrimary);
    Communication()()->SendEventMoveItemPrimaryToField(pCharacter->GetIndex(), originItemName);

    // ui 빼기
    m_pWeapon1->pUIImage = nullptr;
    m_pWeapon1->pItem = nullptr;
    m_pWeapon1->SetIsActive(false);
}

void Character::TotalInventory::EquipPrimary(Item* pNewItem)
{
    const int boxID = EquipItem(&m_pWeaponPrimary, pNewItem);
    if (boxID > -1) // in box
    {
        Communication()()->SendEventMoveItemBoxToPrimary(pCharacter->GetIndex(), boxID, pNewItem->GetName());
    }
    else // in field
    {
        Communication()()->SendEventMoveItemFieldToPrimary(pCharacter->GetIndex(), pNewItem->GetName());
    }

    // ui 끼우기
    m_pWeapon1->pUIImage = m_pWeaponPrimary->GetUIImage2();
    m_pWeapon1->pItem = m_pWeaponPrimary;
    m_pWeapon1->SetIsActive(true);
}

void Character::TotalInventory::DropSecondary()
{
    if (!m_pWeaponSecondary) return;

    const std::string originItemName = m_pWeaponSecondary->GetName();

    ReleaseBullets(m_pWeaponSecondary);
    DropItem(&m_pWeaponSecondary);
    Communication()()->SendEventMoveItemSecondaryToField(pCharacter->GetIndex(), originItemName);

    // ui 빼기
    m_pWeapon2->pUIImage = nullptr;
    m_pWeapon2->pItem = nullptr;
    m_pWeapon2->SetIsActive(false);
}

void Character::TotalInventory::EquipSecondary(Item* pNewItem)
{
    const int boxID = EquipItem(&m_pWeaponSecondary, pNewItem);
    if (boxID > -1) // in box
    {
        Communication()()->SendEventMoveItemBoxToSecondary(pCharacter->GetIndex(), boxID, pNewItem->GetName());
    }
    else // in field
    {
        Communication()()->SendEventMoveItemFieldToSecondary(pCharacter->GetIndex(), pNewItem->GetName());
    }

    // ui 끼우기
    m_pWeapon2->pUIImage = m_pWeaponSecondary->GetUIImage2();
    m_pWeapon2->pItem = m_pWeaponSecondary;
    m_pWeapon2->SetIsActive(true);
}

void Character::TotalInventory::DropArmor()
{
    if (!m_pEquipArmor) return;

    const std::string originItemName = m_pEquipArmor->GetName();

    DropItem(&m_pEquipArmor);
    Communication()()->SendEventMoveItemArmorToField(pCharacter->GetIndex(), originItemName);

    // ui 빼기
    m_pUIArmor->pUIImage = nullptr;
    m_pUIArmor->pItem = nullptr;
    m_pUIArmor->SetIsActive(false);
}

void Character::TotalInventory::EquipArmor(Item* pNewItem)
{
    const int boxID = EquipItem(&m_pEquipArmor, pNewItem);
    if (boxID > -1) // in box
    {
        Communication()()->SendEventMoveItemBoxToArmor(pCharacter->GetIndex(), boxID, pNewItem->GetName());
    }
    else // in field
    {
        Communication()()->SendEventMoveItemFieldToArmor(pCharacter->GetIndex(), pNewItem->GetName());
    }

    // ui 끼우기
    m_pUIArmor->pUIImage = m_pEquipArmor->GetUIImage();
    m_pUIArmor->pItem = m_pEquipArmor;
    m_pUIArmor->SetIsActive(true);
}

void Character::TotalInventory::DropHead()
{
    if (!m_pEquipHead) return;

    const std::string originItemName = m_pEquipHead->GetName();

    DropItem(&m_pEquipHead);
    Communication()()->SendEventMoveItemHeadToField(pCharacter->GetIndex(), originItemName);

    // ui 빼기
    m_pUIHead->pUIImage = nullptr;
    m_pUIHead->pItem = nullptr;
    m_pUIHead->SetIsActive(false);
}

void Character::TotalInventory::EquipHead(Item* pNewItem)
{
    const int boxID = EquipItem(&m_pEquipHead, pNewItem);
    if (boxID > -1) // in box
    {
        Communication()()->SendEventMoveItemBoxToHead(pCharacter->GetIndex(), boxID, pNewItem->GetName());
    }
    else // in field
    {
        Communication()()->SendEventMoveItemFieldToHead(pCharacter->GetIndex(), pNewItem->GetName());
    }

    // ui 끼우기
    m_pUIHead->pUIImage = m_pEquipHead->GetUIImage();
    m_pUIHead->pItem = m_pEquipHead;
    m_pUIHead->SetIsActive(true);
}

void Character::TotalInventory::DropBack()
{
    if (!m_pEquipBack) return;

    const std::string originItemName = m_pEquipBack->GetName();

    DropItem(&m_pEquipBack);
    Communication()()->SendEventMoveItemBackToField(pCharacter->GetIndex(), originItemName);

    // ui 빼기
    m_pUIBack->pUIImage = nullptr;
    m_pUIBack->pItem = nullptr;
    m_pUIBack->SetIsActive(false);
}

void Character::TotalInventory::EquipBack(Item* pNewItem)
{
    const int boxID = EquipItem(&m_pEquipBack, pNewItem);
    if (boxID > -1) // in box
    {
        Communication()()->SendEventMoveItemBoxToBack(pCharacter->GetIndex(), boxID, pNewItem->GetName());
    }
    else // in field
    {
        Communication()()->SendEventMoveItemFieldToBack(pCharacter->GetIndex(), pNewItem->GetName());
    }

    // ui 끼우기
    m_pUIBack->pUIImage = m_pEquipBack->GetUIImage();
    m_pUIBack->pItem = m_pEquipBack;
    m_pUIBack->SetIsActive(true);
}

void Character::TotalInventory::ReleaseBullets(Item* pItem)
{
    if (!pItem) return;

    Item* pAmmo = nullptr;
    int numBullet = 0;
    const auto tag = pItem->GetTagResStatic();
    TAG_RES_STATIC tagBullet = TAG_RES_STATIC::COUNT;
    if (tag == TAG_RES_STATIC::Kar98k)
    {
        numBullet = pItem->GetNumBullet();
        if (numBullet == 0) return;

        tagBullet = TAG_RES_STATIC::Ammo_7_62mm;
    }
    else if (tag == TAG_RES_STATIC::QBZ)
    {
        numBullet = pItem->GetNumBullet();
        if (numBullet == 0) return;

        tagBullet = TAG_RES_STATIC::Ammo_5_56mm;
    }
    else
    {
        // 총이 아님
        return;
    }

    if (!m_mapInventory[tagBullet].empty()) // 인벤토리에 총알 객체가 있을 때
    {
        pAmmo = m_mapInventory[tagBullet].back();
    }
    else // 인벤토리에 총알 객체가 업을 때
    {
        if (!m_empties[tagBullet].empty())
        {
            pAmmo = m_empties[tagBullet].back();
            m_empties[tagBullet].pop_back();
            m_mapInventory[tagBullet].emplace_back(pAmmo);
        }
        else
        {
            assert(
                false && 
                "Character::TotalInventory::ReleaseBullets(), empties is empty");
        }
    }

    pItem->SetNumBullet(0);

    const int numCount = pAmmo->GetCount();
    pAmmo->SetCount(numCount + numBullet);
}

bool Character::PutItemInTotalInventory(Item* item)
{
    assert(
        item && 
        "Character::PutItemInTotalInventory(), item is null.");

    TotalInventory& inven = m_totalInventory;

    TAG_RES_STATIC    tag      = item->GetTagResStatic();
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(tag);

    switch (category)
    {
    case TAG_ITEM_CATEGORY::Ammo:
        {
            if (createOrMergeItem(&inven.m_mapInventory, item))
            {
            }
            else
            {
                return false;
            }
        }
        break;

    case TAG_ITEM_CATEGORY::Attach:
    case TAG_ITEM_CATEGORY::Consumable:
        {
            if (tag == TAG_RES_STATIC::Bandage ||
                tag == TAG_RES_STATIC::FirstAidKit)
            {
                if (createOrMergeItem(&inven.m_mapInventory, item))
                {
                }
                else
                {
                    return false;
                }
            }
            else // attach, medkit
            {
                const float capacity = ItemInfo::GetCapacity(tag);

                if (inven.m_capacity - capacity >= 0.0f)
                {
                    inven.m_mapInventory[tag].push_back(item);
                    inven.m_capacity -= capacity;

                    if (item->IsInDeathDropBox())
                    {
                        const int boxID = item->GetDeathDropBoxIndex();
                        item->DeleteThisInDeathDropBox();
                        Communication()()->SendEventMoveItemBoxToInventory(m_index, boxID, item->GetName());
                    }
                    else
                    {
                        const std::size_t cellIndex = CurrentScene()()->GetCellIndex(item->GetTransform()->GetPosition());
                        CurrentScene()()->ItemIntoInventory(cellIndex, item);
                        CurrentScene()()->RemoveObject(item);
                        Communication()()->SendEventMoveItemFieldToInventory(m_index, item->GetName());
                    }
                }
                else
                {
                    m_inGameUI.pInfoText->SetText("공간이 충분하지 않습니다!", m_inGameUI.pInfoTextShadow);

                    return false;
                }
            }
        }
        break;

    case TAG_ITEM_CATEGORY::Armor:
        {
            //TODO: 피해감소량 적용

            inven.DropArmor();
            inven.EquipArmor(item);

            m_isEatEquip = true;
        }
        break;

    case TAG_ITEM_CATEGORY::Back:
        {
            inven.DropBack();
            inven.EquipBack(item);

            m_isEatEquip = true;
        }
        break;

    case TAG_ITEM_CATEGORY::Head:
        {
            //TODO: 피해감소량 적용

            inven.DropHead();
            inven.EquipHead(item);

            m_isEatEquip = true;
        }
        break;

    case TAG_ITEM_CATEGORY::Rifle:
        //TODO: 무기창의 어느부분에 넣느냐에 따라서 달라짐
        {
            if (inven.m_pHand) // 손에 총을 들고 있을 때
            {
                if (inven.m_handState == TAG_RIFLE::Primary) // 그 총이 프라이머리에서 꺼낸 것일 때
                {
                    if (inven.m_pWeaponSecondary) // 세컨더리에 총이 있다면 
                    {
                        MoveItemHandToPrimary();
                        Communication()()->SendEventMoveItemHandToPrimary(m_index);

                        inven.DropPrimary();
                        inven.pTempSaveWeaponForX = nullptr;

                        inven.EquipPrimary(item);
                        inven.pTempSaveWeaponForX = item;

                        MoveItemPrimaryToHand();
                        Communication()()->SendEventMoveItemPrimaryToHand(m_index);
                    }
                    else
                    {
                        inven.EquipSecondary(item);
                    }
                }
                else if (inven.m_handState == TAG_RIFLE::Secondary) // 그 총이 세컨더리에서 꺼닌 것일 때
                {
                    if (inven.m_pWeaponPrimary) // 프라이머리에 총이 있다면
                    {
                        MoveItemHandToSecondary();
                        Communication()()->SendEventMoveItemHandToSecondary(m_index);

                        inven.DropSecondary();
                        inven.pTempSaveWeaponForX = nullptr;

                        inven.EquipSecondary(item);
                        inven.pTempSaveWeaponForX = item;

                        MoveItemSecondaryToHand();
                        Communication()()->SendEventMoveItemSecondaryToHand(m_index);
                    }
                    else
                    {
                        inven.EquipPrimary(item);
                    }
                }
                else // None
                {
                    assert(
                        false && 
                        "Character::PutItemInTotalInventory(), hand state == None");
                }
            }
            else // 총을 들고 있지 않을 때
            {
                if (inven.m_pWeaponPrimary && 
                    inven.m_pWeaponSecondary) // 프라이머리, 세컨더리 둘 다 총이 있을 때
                {
                    if (inven.pTempSaveWeaponForX) // 총을 들었던 적이 있을 때
                    {
                        const std::string saveName = inven.pTempSaveWeaponForX->GetName();
                        const std::string primaryName = inven.m_pWeaponPrimary->GetName();
                        const std::string secondaryName = inven.m_pWeaponSecondary->GetName();
                        
                        if (saveName == primaryName) // 들었던 적이 있는 총이 프라이머리 총일 때
                        {
                            inven.DropPrimary();
                            inven.pTempSaveWeaponForX = nullptr;

                            inven.EquipPrimary(item);
                            inven.pTempSaveWeaponForX = item;
                        }
                        else if (saveName == secondaryName) // 들었던 적이 있는 총이 세컨더리 총일 때
                        {
                            inven.DropSecondary();
                            inven.pTempSaveWeaponForX = nullptr;

                            inven.EquipSecondary(item);
                            inven.pTempSaveWeaponForX = item;
                        }
                        else // 들었던 적이 있는 총이 프라이머리, 세컨더리 총이 아닐 때
                        {
                            assert(
                                false && 
                                "Character::PutItemInTotalInventory(), saveName != primaryName, secondaryName");
                        }
                    }
                    else // 총을 들었던 적이 없을 때 /* 기존 배그에 없는 추가한 예외처리 */
                    {
                        inven.DropPrimary();
                        inven.EquipPrimary(item);
                    }
                }
                else if (inven.m_pWeaponPrimary) // 프라이머리에만 총이 있을 때
                {
                    inven.EquipSecondary(item);
                }
                else if (inven.m_pWeaponSecondary) // 세컨더리에만 총이 있을 때
                {
                    inven.EquipPrimary(item);
                }
                else // 프라이머리, 세컨더리 둘 다 총이 없을 때
                {
                    inven.EquipPrimary(item);
                }
            }
        }
        break;

    default:
        assert(false && "PutItemInTotalInventory(), default case.");
        break;
    }

    return true;

    //For Debug
    ShowTotalInventory();
}

bool Character::createOrMergeItem(std::map<TAG_RES_STATIC, std::vector<Item*>>* map, Item* item)
{
    /*
        - 자신의 용량만큼 캐릭터의 소지용량에서 제외된다
        - 소모품(Bandage, FirstAidKit), 탄약
    */
    assert(
        map && 
        item && 
        "Character::CreateOrMergeItem(), argument is null.");

    TAG_RES_STATIC tag = item->GetTagResStatic();
    auto it = map->find(tag);
    int count = item->GetCount();
    const float capacity = ItemInfo::GetCapacity(tag);

    if (m_totalInventory.m_capacity - count * capacity >= 0.0f)
    {
        if (it == map->end())
        {
            // 아이템이 없는 경우는 새로 생성한다
            (*map)[tag].push_back(item);
            item->SetCount(count);
            item->SetIsInInventory(true);

            if (item->IsInDeathDropBox())
            {
                const int boxID = item->GetDeathDropBoxIndex();
                item->DeleteThisInDeathDropBox();
                Communication()()->SendEventMoveItemBoxToInventory(m_index, boxID, item->GetName());
            }
            else // in field
            {
                const std::size_t cellIndex = CurrentScene()()->GetCellIndex(item->GetTransform()->GetPosition());
                CurrentScene()()->ItemIntoInventory(cellIndex, item);
                CurrentScene()()->RemoveObject(item);
                Communication()()->SendEventMoveItemFieldToInventory(m_index, item->GetName());
            }
        }
        else
        {
            // 이미 인벤토리에 있는 경우, 기존 개수와 합친다
            auto origin_item = it->second.back();
            origin_item->SetCount(origin_item->GetCount() + count);
            item->SetCount(0);
            m_totalInventory.m_empties[tag].emplace_back(item);

            if (item->IsInDeathDropBox())
            {
                const int boxID = item->GetDeathDropBoxIndex();
                item->DeleteThisInDeathDropBox();
                Communication()()->SendEventMoveItemBoxToInventory(m_index, boxID, item->GetName());
            }
            else // in field
            {
                const std::size_t cellIndex = CurrentScene()()->GetCellIndex(item->GetTransform()->GetPosition());
                CurrentScene()()->ItemIntoInventory(cellIndex, item);
                CurrentScene()()->RemoveObject(item);

                // OOTZ_FLAG : 네트워크 필드 -> 밴디지, 퍼스트에이드키트, 탄약
                Communication()()->SendEventMoveItemFieldToInventory(m_index, item->GetName());
            }
        }
        m_totalInventory.m_capacity -= count * capacity;
    }
    else
    {
        m_inGameUI.pInfoText->SetText(
            "공간이 충분하지 않습니다!", 
            m_inGameUI.pInfoTextShadow);

        return false;
    }

    return true;
}

bool Character::checkOriginItem(Item** originItem, Item* newItem)
{
    /*
        - 장비, 무기
        - 소지용량에 영향을 끼치지 않으면서, 템에따라 용량을 늘린다(조끼, 배낭)
    */
    assert(newItem && "Character::checkOriginItem(), argument is null.");

    if (*originItem) // 기존에 아이템이 있으면
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory((*originItem)->GetTagResStatic());

        //TODO: 바닥에 떨군다
        Item* pItem = *originItem;
        pItem->SetIsRenderEffectMesh(true);
        pItem->SetIsRenderSkinnedMesh(false);
        pItem->GetTransform()->SetRotation(Vector3::ZERO);
        pItem->GetTransform()->Update();

        //용량을 줄인다
        m_totalInventory.m_capacity -= ItemInfo::GetCapacityExtension(newItem->GetTagResStatic());
        if (category == TAG_ITEM_CATEGORY::Armor ||
            category == TAG_ITEM_CATEGORY::Back  ||
            category == TAG_ITEM_CATEGORY::Head)
            m_totalInventory.m_equipOnNum--;

        return true;
    }
    else
    {
        *originItem = newItem;
        //m_totalInventory.m_capacity += ItemInfo::GetCapacity(newItem->GetTagResStatic());
        //용량을 늘린다
        m_totalInventory.m_capacity += ItemInfo::GetCapacityExtension(newItem->GetTagResStatic());

        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(newItem->GetTagResStatic());
        if (category == TAG_ITEM_CATEGORY::Armor ||
            category == TAG_ITEM_CATEGORY::Back ||
            category == TAG_ITEM_CATEGORY::Head)
            m_totalInventory.m_equipOnNum++;

        // eqiup
        newItem->SetIsInInventory(true);
        newItem->SetIsRenderEffectMesh(false);
        newItem->SetIsRenderSkinnedMesh(true);

        return false;
    }
}

void Character::updateTotalInventory()
{
    Transform* pTr = GetTransform();

    Item* pHand = m_totalInventory.m_pHand;
    if (pHand)
    {
        Transform* pHandTr = pHand->GetTransform();
        D3DXMATRIX weaponWorld = 
            m_framePtr.pHandGun->CombinedTransformationMatrix
            * pTr->GetTransformationMatrix();

        pHandTr->SetTransformationMatrix(weaponWorld);
        pHandTr->Update();

        //총 자체 애니메이션을 업데이트한다
        if (m_isNeedRifleAnim)
            pHand->UpdateAnimation();

        pHand->UpdateModel();

        Shader()()->AddShadowSource(
            pHandTr->GetTransformationMatrix(), 
            pHand->GetSkinnedMesh());
    }

    Item* pArmor = m_totalInventory.m_pEquipArmor;
    if (pArmor)
    {
        Transform* pArmorTr = pArmor->GetTransform();
        D3DXMATRIX armorWorld = pTr->GetTransformationMatrix();

        pArmorTr->SetTransformationMatrix(armorWorld);
        pArmorTr->Update();

        pArmor->UpdateAnimation();

        pArmor->UpdateBone(
            m_totalInventory.m_pHand,
            m_headRotation.m_angle,
            m_waistRotation.m_angle);

        pArmor->UpdateModel();
    }

    Item* pBack = m_totalInventory.m_pEquipBack;
    if (pBack)
    {
        Transform* pBackTr = pBack->GetTransform();
        D3DXMATRIX backWorld = pTr->GetTransformationMatrix();

        pBackTr->SetTransformationMatrix(backWorld);
        pBackTr->Update();

        pBack->UpdateAnimation();

        pBack->UpdateBone(
            m_totalInventory.m_pHand,
            m_headRotation.m_angle,
            m_waistRotation.m_angle);

        pBack->UpdateModel();
    }

    Item* pHead = m_totalInventory.m_pEquipHead;
    if (pHead)
    {
        Transform* pHeadTr = pHead->GetTransform();
        D3DXMATRIX headWorld = pTr->GetTransformationMatrix();

        pHeadTr->SetTransformationMatrix(headWorld);
        pHeadTr->Update();

        pHead->UpdateAnimation();

        pHead->UpdateBone(
            m_totalInventory.m_pHand, 
            m_headRotation.m_angle, 
            m_waistRotation.m_angle);

        pHead->UpdateModel();

        //Shader()()->AddShadowSource(
        //    pHeadTr->GetTransformationMatrix(),
        //    pHead->GetSkinnedMesh());
    }

    //주무기
    Item* pWeaponPrimary = m_totalInventory.m_pWeaponPrimary;
    if (pWeaponPrimary)
    {
        Transform* pWeaponPrimaryTr = pWeaponPrimary->GetTransform();

        // 모델스페이스 m_framePtr.pSlotPrimary->CombinedTransformationMatrix
        // 월드스페이스 = 모델스페이스 * 캐릭터 월드스페이스

        D3DXMATRIX weaponWorld = 
            m_framePtr.pSlotPrimary->CombinedTransformationMatrix
            * pTr->GetTransformationMatrix();

        pWeaponPrimaryTr->SetTransformationMatrix(weaponWorld);

        //Debug << "primary weapon matrix : \n" << weaponWorld << '\n';

        pWeaponPrimaryTr->Update();

        Shader()()->AddShadowSource(
            pWeaponPrimaryTr->GetTransformationMatrix(), 
            pWeaponPrimary->GetSkinnedMesh());
    }
    
    //보조무기
    Item* pWeaponSecondary = m_totalInventory.m_pWeaponSecondary;
    if (pWeaponSecondary)
    {
        Transform* pWeaponSecondaryTr = pWeaponSecondary->GetTransform();
        D3DXMATRIX weaponWorld = m_framePtr.pSlotSecondary->CombinedTransformationMatrix
            * pTr->GetTransformationMatrix();

        pWeaponSecondaryTr->SetTransformationMatrix(weaponWorld);

        //Debug << "secondary weapon matrix : \n" << weaponWorld << '\n';

        pWeaponSecondaryTr->Update();

        Shader()()->AddShadowSource(
            pWeaponSecondaryTr->GetTransformationMatrix(),
            pWeaponSecondary->GetSkinnedMesh());
    }
}

void Character::renderTotalInventory()
{
    Item* pHand = m_totalInventory.m_pHand;
    if (pHand) pHand->Render();

    Item* pArmor = m_totalInventory.m_pEquipArmor;
    if (pArmor) pArmor->Render();

    Item* pBack = m_totalInventory.m_pEquipBack;
    if (pBack) pBack->Render();

    Item* pHead = m_totalInventory.m_pEquipHead;
    if (pHead) pHead->Render();

    Item* pWeaponPrimary = m_totalInventory.m_pWeaponPrimary;
    if (pWeaponPrimary) pWeaponPrimary->Render();

    Item* pWeaponSecondary = m_totalInventory.m_pWeaponSecondary;
    if (pWeaponSecondary) pWeaponSecondary->Render();
}

void Character::ShowTotalInventory()
{
    TAG_RES_STATIC tag;

    cout << "기본용량: " << Character::TotalInventory::DEFAULT_CAPACITY << endl;
    cout << "남은용량: " << m_totalInventory.m_capacity << endl;
    cout << "<인벤토리>" << endl;
    for (auto items : m_totalInventory.m_mapInventory)
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        for (auto item : items.second)
        {
            tag = item->GetTagResStatic();
            cout << "- " << ItemInfo::GetName(tag);
            cout << " " << item->GetCount() << "개";
            cout << " 용량: " << ItemInfo::GetCapacity(tag);
            cout << endl;
        }
    }

    TAG_ITEM_CATEGORY category;
    cout << "<장비>" << endl;

    if (m_totalInventory.m_pEquipArmor)
    {
        tag = m_totalInventory.m_pEquipArmor->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pEquipArmor->GetCount() << "개";
        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_pEquipBack)
    {
        tag = m_totalInventory.m_pEquipBack->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pEquipBack->GetCount() << "개";
        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_pEquipHead)
    {
        tag = m_totalInventory.m_pEquipHead->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pEquipHead->GetCount() << "개";
        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    }

    cout << "<무기>" << endl;

    if (m_totalInventory.m_pWeaponPrimary)
    {
        tag = m_totalInventory.m_pWeaponPrimary->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pWeaponPrimary->GetCount() << "개";
        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_pWeaponSecondary)
    {
        tag = m_totalInventory.m_pWeaponSecondary->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pWeaponSecondary->GetCount() << "개";
        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    }
}

