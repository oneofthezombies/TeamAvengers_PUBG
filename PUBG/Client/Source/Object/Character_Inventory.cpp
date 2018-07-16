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

using Event       = UIButtonWithItem::Event;
using MouseButton = UIButtonWithItem::MouseButton;

const float Character::TotalInventory::DEFAULT_CAPACITY = 70.0f;

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
    , m_pUIPicked(nullptr)
    , m_pWeapon1(nullptr)
    , m_pWeapon2(nullptr)
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
              
    if(m_pWeaponPrimary)SAFE_DELETE(m_pWeaponPrimary);
    SAFE_DELETE(m_pWeaponSecondary);
}


void Character::TotalInventory::Init()
{
    // 검정 배경
    m_Border = new UIImage(
        "./Resource/UI/Inventory/Basic/",
        "black_1280_720_70.png",
        Vector3::ZERO,
        nullptr,
        nullptr);
    UI()()->RegisterUIObject(m_Border);

    // 텍스트
    auto a = new UIText(
        Resource()()->GetFont(TAG_FONT::Invetory_28),
        D3DXVECTOR2(100.0f, 20.0f),
        string("VICINITY"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(74.0f, 24.0f, 0.0f));

    // 텍스트
    a = new UIText(
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        D3DXVECTOR2(100.0f, 20.0f),
        string("Ground"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(74.0f, 77.0f, 0.0f));

    // 텍스트 
    a = new UIText(
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        D3DXVECTOR2(100.0f, 20.0f),
        string("Sortby Type"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(264.0f, 77.0f, 0.0f));

    // 드롭드 인벤 구분선
    new UIImage(
        "./Resource/UI/Inventory/Basic/",
        "line.png",
        D3DXVECTOR3(233.0f, 92.0f, 0.0f),
        nullptr,
        m_Border);

    // 여자 캐릭터
    auto b = new UIImage(
        "./Resource/UI/Inventory/Character/",
        "Female.png",
        D3DXVECTOR3(410.0f, 85.0f, 0.0f),
        nullptr,
        m_Border);

    SetEquipUI();

    m_Border->SetIsRender(false);
}

void Character::TotalInventory::Destroy()
{
    UI()()->Destroy(m_Border);
    UI()()->Destroy(m_pUIPicked);
    UI()()->Destroy(m_pWeapon1);
    UI()()->Destroy(m_pWeapon2);
}

void Character::TotalInventory::Open()
{
    isOpened = true;
    // move ui to in screen

    m_Border->SetIsRender(isOpened);
}

void Character::TotalInventory::Close()
{
    isOpened = false;
    // move ui to out screen
    m_Border->SetIsRender(isOpened);

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
            //pUI->SetText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
            m_Border,
            Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
    const float max = std::numeric_limits<float>::max();
    m_pUIPicked = new UIButtonWithItem(
        Vector3::ONE * max,
        "./Resource/UI/Inventory/Basic/",
        "Equip_click.png",
        "Equip_click.png",
        "Equip_click.png",
        nullptr, nullptr, "", 0, nullptr,
        TAG_UI_POSITION::picked,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    UI()()->RegisterUIObject(m_pUIPicked);

    new UIImage("", "", Vector3::ZERO, nullptr, m_pUIPicked);

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
            m_Border,
            Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
    auto u = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        m_Border,
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
     u = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        m_Border,
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
     u = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        m_Border,
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
     u = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_click.png",
        m_Border,
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
        nullptr, m_Border);
    m_pWeapon1 = new UIButtonWithItem(
        D3DXVECTOR3(
            static_cast<float>(left),
            static_cast<float>(top), 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "WeaponBox_idle.png",
        "WeaponBox_mouseover.png",
        "WeaponBox_possible.png",
        m_Border,
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
        nullptr, m_Border);

    
        m_pWeapon2 = new UIButtonWithItem(
         D3DXVECTOR3(
             static_cast<float>(left),
             static_cast<float>(top), 0.0f),
         "./Resource/UI/Inventory/Basic/",
         "WeaponBox_idle.png",
         "WeaponBox_mouseover.png",
         "WeaponBox_possible.png",
         m_Border,
         Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
         m_Border,
         Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
     new UIText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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
     new UIText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
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

void Character::PutItemInTotalInventory(Item* item)
{
    assert(item && "Character::PutItemInTotalInventory(), item is null.");

    TAG_RES_STATIC    tag      = item->GetTagResStatic();
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(tag);

    switch (category)
    {
    case TAG_ITEM_CATEGORY::Ammo:
        createOrMergeItem(&(m_totalInventory.m_mapInventory), item);
        break;

    case TAG_ITEM_CATEGORY::Attach:
    case TAG_ITEM_CATEGORY::Consumable:
    {
        if (tag == TAG_RES_STATIC::Bandage ||
            tag == TAG_RES_STATIC::FirstAidKit)
        {
            createOrMergeItem(&(m_totalInventory.m_mapInventory), item);
        }
        else
        {
            if (m_totalInventory.m_capacity - ItemInfo::GetCapacity(tag) >= 0)
            {
                (m_totalInventory.m_mapInventory)[tag].push_back(item);
                CurrentScene()()->RemoveObject(item);
                m_totalInventory.m_capacity -= ItemInfo::GetCapacity(tag);
            }
            else
            {
                m_inGameUI.pInfoText->SetText("공간이 충분하지 않습니다!", m_inGameUI.pInfoTextShadow);
            }
            // TODO : send "delete item on field" to server
        }
    }
        break;

    case TAG_ITEM_CATEGORY::Armor:
        //TODO: 피해감소량 적용
        checkOriginItem(&m_totalInventory.m_pEquipArmor, item);
        break;

    case TAG_ITEM_CATEGORY::Back:
        checkOriginItem(&m_totalInventory.m_pEquipBack, item);
        break;

    case TAG_ITEM_CATEGORY::Head:
        //TODO: 피해감소량 적용
        checkOriginItem(&m_totalInventory.m_pEquipHead, item);
        break;

    case TAG_ITEM_CATEGORY::Rifle:
        //TODO: 무기창의 어느부분에 넣느냐에 따라서 달라짐
        //지금은 임시로 QBZ를 주무기, Kar98k를 보조무기로
        {
            if(!m_totalInventory.m_pWeaponPrimary &&
                !(m_totalInventory.m_handState==TAG_RIFLE::Primary))
            {
                checkOriginItem((Item**)&m_totalInventory.m_pWeaponPrimary, item);
            }
            else
            {
                checkOriginItem((Item**)&m_totalInventory.m_pWeaponSecondary, item);
            }
        }
        break;

    default:
        assert(false && "PutItemInTotalInventory(), default case.");
        break;
    }

    //For Debug
    ShowTotalInventory();
}

void Character::createOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item)
{
    /*
        - 자신의 용량만큼 캐릭터의 소지용량에서 제외된다
        - 소모품, 부착물, 탄약
    */
    assert(map && item && "Character::CreateOrMergeItem(), argument is null.");

    TAG_RES_STATIC tag = item->GetTagResStatic();
    auto it = map->find(tag);
    int count = item->GetCount();

    if (m_totalInventory.m_capacity - count * ItemInfo::GetCapacity(tag) >= 0)
    {
        if (it == map->end())
        {
            //아이템이 없는 경우는 새로 생성한다
            (*map)[tag].push_back(item);
            item->SetCount(count);
            CurrentScene()()->RemoveObject(item);
            // TODO : send "delete item on field" to server
        }
        else
        {
            //이미 인벤토리에 있는 경우, 기존 개수와 합친다
            auto origin_item = it->second.back();
            origin_item->SetCount(origin_item->GetCount() + count);
            CurrentScene()()->Destroy(item);
            // TODO : send "delete item on field" to server
        }
        m_totalInventory.m_capacity -= count * ItemInfo::GetCapacity(tag);
    }
    else
    {
        m_inGameUI.pInfoText->SetText("공간이 충분하지 않습니다!", m_inGameUI.pInfoTextShadow);
    }

}

void Character::checkOriginItem(Item** originItem, Item* newItem)
{
    /*
        - 장비, 무기
        - 소지용량에 영향을 끼치지 않으면서, 템에따라 용량을 늘린다(조끼, 배낭)
    */
    assert(newItem && "Character::checkOriginItem(), argument is null.");
    if (*originItem)
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory((*originItem)->GetTagResStatic());

        (*originItem)->SetIsRenderEffectMesh(true);
        //TODO: 바닥에 떨군다
        //용량을 줄인다
        m_totalInventory.m_capacity -= ItemInfo::GetCapacityExtension(newItem->GetTagResStatic());
        if (category == TAG_ITEM_CATEGORY::Armor ||
            category == TAG_ITEM_CATEGORY::Back  ||
            category == TAG_ITEM_CATEGORY::Head)
            m_totalInventory.m_equipOnNum--;

        //아래는 임시코드
        CurrentScene()()->Destroy(*originItem);
        checkOriginItem(originItem, newItem);
    }
    else
    {
        *originItem = newItem;
        //m_totalInventory.m_capacity += ItemInfo::GetCapacity(newItem->GetTagResStatic());
        //용량을 늘린다
        m_totalInventory.m_capacity += ItemInfo::GetCapacityExtension(newItem->GetTagResStatic());
        CurrentScene()()->RemoveObject(newItem);
        
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(newItem->GetTagResStatic());
        if (category == TAG_ITEM_CATEGORY::Armor ||
            category == TAG_ITEM_CATEGORY::Back ||
            category == TAG_ITEM_CATEGORY::Head)
            m_totalInventory.m_equipOnNum++;

        // eqiup
        newItem->SetIsRenderEffectMesh(false);
        newItem->SetIsRenderSkinnedMesh(true);
        // TODO : send "delete item on field" to server
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

void Character::onMouse(
    const Event event, 
    const MouseButton button, 
    UIButtonWithItem* pUIButtonWithItem)
{
    assert(pUIButtonWithItem && "Character::onMouse() button is null.");

    auto& ti = m_totalInventory;
    const auto tag = pUIButtonWithItem->m_tagUIPosition;

    if (button == MouseButton::LEFT)
    {
        if (event == Event::DOWN && !ti.m_stateClicked)
        {
            if (UIPosition::IsDropped(tag)|| UIPosition::IsInven(tag))
            {
                // 좌표
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

                ti.m_pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
                ti.m_pUIPicked->pItem = pUIButtonWithItem->pItem;

                UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage();
                UIImage* pPickedImage = static_cast<UIImage*>(ti.m_pUIPicked->GetChild(0));

                pPickedImage->SetTexture(pItemImage->GetTexture());
                pPickedImage->SetSize(pItemImage->GetSize());

                if (ItemInfo::GetItemCategory(ti.m_pUIPicked->pItem->GetTagResStatic()) == TAG_ITEM_CATEGORY::Rifle)
                {
                    ti.m_pWeapon1->SetIsActive(true);
                    ti.m_pWeapon2->SetIsActive(true);
                }
            }
            //if(UIPosition::IsInven(tag))

            if (tag == TAG_UI_POSITION::Weapon1)
            {
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

                ti.m_pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
                ti.m_pUIPicked->pItem = pUIButtonWithItem->pItem;

                UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage();
                UIImage* pPickedImage = static_cast<UIImage*>(ti.m_pUIPicked->GetChild(0));

                pPickedImage->SetTexture(pItemImage->GetTexture());
                pPickedImage->SetSize(pItemImage->GetSize());
            }
            else if (tag == TAG_UI_POSITION::Weapon2)
            {
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

                ti.m_pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
                ti.m_pUIPicked->pItem = pUIButtonWithItem->pItem;

                UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage();
                UIImage* pPickedImage = static_cast<UIImage*>(ti.m_pUIPicked->GetChild(0));

                pPickedImage->SetTexture(pItemImage->GetTexture());
                pPickedImage->SetSize(pItemImage->GetSize());
            }
            ti.m_stateClicked = true;

        }

        if (event == Event::DRAG)
        {
            if (tag == TAG_UI_POSITION::picked)
            {
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

                ti.m_pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
            }
        }

        if (event == Event::UP)
        {
            if (tag == TAG_UI_POSITION::picked 
                && !pUIButtonWithItem->pItem->GetState())
            {
                const D3DXVECTOR3& pos = pUIButtonWithItem->GetPosition();
                const auto cat =
                    ItemInfo::GetItemCategory(
                        pUIButtonWithItem->pItem->GetTagResStatic());

                // 드롭드 인벤 구분선 위치
                const float firstLine = 233.0f;
                // TODO : 인벤 장착 구분선 위치 조정해야 함
                const float secondLine = 500.0f;
                if (pos.x < firstLine)
                {
                }
                else if (pos.x < secondLine)
                {
                    switch (cat)
                    {
                        case TAG_ITEM_CATEGORY::Ammo:
                        {
                            Item* pItem = pUIButtonWithItem->pItem;
                            PutItemInTotalInventory(pItem);
                            pItem->SetState(true);
                            CurrentScene()()->ItemIntoInventory(
                                CurrentScene()()->GetCellIndex(
                                    pItem->GetTransform()->GetPosition()),
                                pItem);
                        }
                        break;

                        case TAG_ITEM_CATEGORY::Rifle:
                        {
                            Item* pItem = pUIButtonWithItem->pItem;
                            PutItemInTotalInventory(pItem);
                            pItem->SetState(true);
                            CurrentScene()()->ItemIntoInventory(
                                CurrentScene()()->GetCellIndex(
                                    pItem->GetTransform()->GetPosition()),
                                pItem);
                            /*UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage2();*/

                            //웨폰 버튼.
                            if (ti.m_pWeaponPrimary)
                            {
                                ti.m_pWeapon1->pUIImage = ti.m_pWeaponPrimary->GetUIImage2();
                                ti.m_pWeapon1->pItem = ti.m_pWeaponPrimary;
                            }
                            
                            if (ti.m_pWeaponSecondary)
                            {
                                ti.m_pWeapon2->pUIImage = ti.m_pWeaponSecondary->GetUIImage2();
                                ti.m_pWeapon2->pItem = ti.m_pWeaponSecondary;
                            }
                        }
                        break;
                    }
                }
                else    //3번째 장비 착용창.
                {
                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                    {
                        Item* pItem = pUIButtonWithItem->pItem;
                        PutItemInTotalInventory(pItem);
                        pItem->SetState(true);
                        CurrentScene()()->ItemIntoInventory(
                            CurrentScene()()->GetCellIndex(
                                pItem->GetTransform()->GetPosition()),
                            pItem);
                    }
                    break;
                    /*case TAG_ITEM_CATEGORY::Rifle:
                    {
                        Item* pItem = pUIButtonWithItem->pItem;
                        PutItemInTotalInventory(pItem);
                        pItem->SetState(true);
                        CurrentScene()()->ItemIntoInventory(
                            CurrentScene()()->GetCellIndex(
                                pItem->GetTransform()->GetPosition()),
                            pItem);
                        UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage2();
                        ti.m_pWeapon1->pUIImage = pItemImage;
                        ti.m_pWeapon1->pItem = pItem;

                    }*/
                    break;
                    }
                }
                const float max = std::numeric_limits<float>::max();
                pUIButtonWithItem->SetPosition(Vector3::ONE * max);
            }
            /////////////////////인벤토리 안에서
            else if (tag == TAG_UI_POSITION::picked
                && pUIButtonWithItem->pItem->GetState())
            {
                const D3DXVECTOR3& pos = pUIButtonWithItem->GetPosition();
                const auto cat =
                    ItemInfo::GetItemCategory(
                        pUIButtonWithItem->pItem->GetTagResStatic());

                // 드롭드 인벤 구분선 위치
                const float firstLine = 233.0f;
                // TODO : 인벤 장착 구분선 위치 조정해야 함
                const float secondLine = 500.0f;
                if (pos.x < firstLine)
                {
                    Item* pItem = pUIButtonWithItem->pItem;
                    
                    pItem->SetState(false);
                    D3DXVECTOR3 p = m_totalInventory.pCharacter->GetTransform()->GetPosition();
                    CurrentScene()()->AddObject(pItem);
                    CurrentScene()()->InsertObjIntoTotalCellSpace(TAG_OBJECT::Item, CurrentScene()()->GetCellIndex(p), pItem);
                    pItem->SetPosition(D3DXVECTOR3(p.x,p.y+20,p.z));
                    ti.m_mapInventory.erase(pItem->GetTagResStatic());
                    pItem->GetTransform()->Update();

                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Rifle:
                        {
                            //if()
                            //바닥에 떨구면 Effect
                            pItem->SetIsRenderSkinnedMesh(false);
                            pItem->SetIsRenderEffectMesh(true);
                            pItem->GetTransform()->SetRotation(Vector3::ZERO);
                            pItem->GetTransform()->Update();
                            
                            if (pItem == ti.m_pWeapon1->pItem)
                            {
                                ti.m_pWeapon1->pUIImage = nullptr;
                                ti.m_pWeapon1->pItem = nullptr;
                                ti.m_pWeaponPrimary = nullptr;
                                if (ti.m_handState == TAG_RIFLE::Primary)
                                {
                                    ti.m_handState = TAG_RIFLE::None;
                                    ti.pTempSaveWeaponForX = nullptr;
                                }

                            }
                            else if(pItem == ti.m_pWeapon2->pItem)
                            {
                                ti.m_pWeapon2->pUIImage = nullptr;
                                ti.m_pWeapon2->pItem = nullptr;
                                ti.m_pWeaponSecondary = nullptr;
                                if (ti.m_handState == TAG_RIFLE::Secondary)
                                {
                                    ti.m_handState = TAG_RIFLE::None;
                                    ti.pTempSaveWeaponForX = nullptr;
                                }
                            }

                            //손에 장착 되어있는 아이템이 클릭한 아이템이랑 같다면 
                            if (pItem == ti.m_pHand)
                            {
                                ti.m_pHand = nullptr;
                                ti.m_handState = TAG_RIFLE::None;
                                ti.pTempSaveWeaponForX = nullptr;
                                m_attacking = Attacking::Unarmed;

                                //캐릭터 애니메이션
                                {

                                    TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
                                    if (m_stance == Stance::Stand)
                                        tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1;
                                    else if (m_stance == Stance::Crouch)
                                        tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1;
                                    else if (m_stance == Stance::Prone)
                                        tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1;
                                    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && " Character::onMouse(), COUNT");

                                    setAnimation(
                                        CharacterAnimation::BodyPart::BOTH,
                                        tagAnim,
                                        true);
                                }

                            }


                            
                        }
                        break;

                    default:
                        break;
                    }
                    //pItem->
                }
                else if (pos.x < secondLine)
                {
                }
                else
                {
                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                    {

                    }
                    break;
                    }
                }
                const float max = std::numeric_limits<float>::max();
                pUIButtonWithItem->SetPosition(Vector3::ONE * max);
            }
            if (ti.m_pWeaponPrimary == nullptr && !(ti.m_handState == TAG_RIFLE::Primary))
            {
                ti.m_pWeapon1->SetIsActive(false);
            }
            if (ti.m_pWeaponSecondary == nullptr && !(ti.m_handState==TAG_RIFLE::Secondary)
                /*&& ti.m_pHand == nullptr*/)
            {
                ti.m_pWeapon2->SetIsActive(false);
            }
            ti.m_stateClicked = false;
        }
    }
   
}
