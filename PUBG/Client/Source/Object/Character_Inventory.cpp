#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
///////추가한거 UI
#include "UIImage.h"
#include "UIText.h"

using Event       = UIButtonWithItem::Event;
using MouseButton = UIButtonWithItem::MouseButton;

const float Character::TotalInventory::DEFAULT_CAPACITY = 70.0f;

Character::TotalInventory::TotalInventory()
    : m_pHand(nullptr)
    , m_pEquipArmor(nullptr)
    , m_pEquipBack(nullptr)
    , m_pEquipHead(nullptr)
    , m_pWeaponPrimary(nullptr)
    , m_pWeaponSecondary(nullptr)
    , m_capacity(DEFAULT_CAPACITY)
    , m_numReload(0)
    , m_bulletFireCoolDown(0)

    , pTempSaveWeaponForX(nullptr)
    , pCharacter(nullptr)
{
    m_Border = new UIImage(
        "./Resource/UI/Inventory/Basic/",
        "black_1280_720_70.png",
        Vector3::ZERO,
        nullptr,
        nullptr);

    auto a = new UIText(
        Resource()()->GetFont(TAG_FONT::Invetory_28),
        D3DXVECTOR2(100.0f, 20.0f),
        //D3DXVECTOR2(260.0f, 70.0f),
        string("VICINITY"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(73.0f, 35.0f, 0.0f));


    a = new UIText(
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        D3DXVECTOR2(100.0f, 20.0f),
        string("Ground"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(73.0f, 75.0f, 0.0f));

    a = new UIText(
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        D3DXVECTOR2(100.0f, 20.0f),
        string("Sortby Type"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);
    a->SetDrawTextFormat(DT_LEFT);
    a->SetPosition(D3DXVECTOR3(243.0f, 75.0f, 0.0f));

    new UIImage(
        "./Resource/UI/Inventory/Basic/",
        "line.png",
        D3DXVECTOR3(233.0f, 93.0f, 0.0f),
        nullptr,
        m_Border);

    ////////////이미지 확인용 나중에 지워도 됨.
    new UIImage
    (
        "./Resource/UI/Inventory/Basic/",
        "Equip_yes.png",
        D3DXVECTOR3(440.0f, 248.0f, 0.0f),
        nullptr,
        m_Border
    );

    //UIButton(const D3DXVECTOR3& pos,
    //    const string& textureDir, const string& idleTex, const string& mouseOverTex, const string& selectTex,
    //    IUIButtonOnMouseListener* pListener, UIObject* pParent,
    //    const LPD3DXFONT font, const string& text, const D3DCOLOR textColor);


    //new UIButton(D3DXVECTOR3(440.0f,117.0f,0.0f),"./Resource/UI/Inventory/Basic","Equip_no.png", "Equip_mouseover", "Equip_yes.png", &m_buttonListener,)

    SetEquipUI();

    float miniPer = 0.67f; //miniPersentage
    auto b = new UIImage(
        "./Resource/UI/Inventory/Character/",
        "female.tga",
        D3DXVECTOR3(410.0f / miniPer, 85.0f / miniPer, 0.0f),
        nullptr,
        m_Border);

    //아이콘 이미지 size 조절
    D3DXMATRIX s;
    D3DXMatrixScaling(&s, miniPer, miniPer, 0.0f);
    b->SetTransform(s);
    


    //Text
    m_Text = new UIText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        D3DXVECTOR2(0.0f, 0.0f),
        string("WTF"),
        D3DCOLOR_XRGB(200, 200, 200),
        m_Border);


    //m_Border->AddChild(m_Text);
    m_Border->SetIsRender(false);
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


void Character::TotalInventory::Open()
{
    isOpened = true;
    // move ui to in screen

    m_Border->SetIsRender(isOpened);
    
    for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); it++)
    {
        (*it).second.back()->SetInRenderUIImage(isOpened);
        (*it).second.back()->GetUIText()->SetText(string(ItemInfo::GetName((*it).first))
            + "  " + to_string((*it).second.back()->GetCount()));
    }

    //if (m_Text->GetChild(0)==nullptr)
    //{
    //    vector<RECT> rects;
    //    int i = 0;
    //    for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); it++)
    //    {
    //        (*it).second.back()->SetUIPosition(D3DXVECTOR2(250.0f / 0.2f, (100.0f + 50.0f * static_cast<float>(i)) / 0.2f));
    //        auto c = new UIText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
    //            D3DXVECTOR2(100.0f, 50.0f),
    //            string(ItemInfo::GetName((*it).first))
    //            + "  " + to_string((*it).second.back()->GetCount()),
    //            D3DCOLOR_XRGB(200, 200, 200),
    //            m_Text);
    //        c->SetDrawTextFormat(DT_LEFT);
    //        c->SetPosition(D3DXVECTOR3(310.0f, 120.0f + 50.0f * static_cast<float>(i), 0.0f));
    //        //Gun* g = it->second;
    //        //UIText::Create(Font::kInteractionMessageDescription, g->GetName(), D3DXVECTOR3(600.0f, 100.0f + 50.0f * static_cast<float>(i), 0.0f), D3DXVECTOR2(200.0f, 50.0f), this);
    //        rects.emplace_back(RECT{});
    //        SetRect(&rects.back(), 350, 100 + 50 * i, 550, 150 + 50 * i);
    //        ++i;
    //    }
    //}
}

void Character::TotalInventory::Close()
{
    isOpened = false;
    // move ui to out screen
    m_Border->SetIsRender(isOpened);
    for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); it++)
    {
        (*it).second.back()->SetInRenderUIImage(isOpened);
    }
    //UI()()->Destroy(m_Border);
    //POINT center;
    //center.x = 1280 / 2;
    //center.y = 720 / 2;
    //ClientToScreen(g_hWnd, &center);
    //SetCursorPos(center.x, center.y);
}

void Character::TotalInventory::Update()
{
    if (isOpened)
    {
        droppedItems.resize(0);

        ////Character player = Scene.GetPlayer();
        //BoundingSphere bs = player.GetBoundingSphere().Move(/* position */);
        //for (item : Camera.GetNearArea(player.GetCellIndex()).GetItems())
        //{
        //    if (Collision::HasCollision(bs, item.GetBoundingSphere()))
        //    {
        //        droppedItems.emplace_back(item);
        //    }
        //}
        
        //vector<RECT> rects;
        int i = 0;
        for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); it++)
        {
            (*it).second.back()->SetUIPosition(D3DXVECTOR2(250.0f / 0.2f, (100.0f + 50.0f * static_cast<float>(i)) / 0.2f));
            //(*it).second.back()->GetUIText()->SetPosition(D3DXVECTOR3(310.0f, 120.0f + 50.0f * static_cast<float>(i), 0.0f));
            (*it).second.back()->GetUIText()->SetText(string(ItemInfo::GetName((*it).first))
                + "  " + to_string((*it).second.back()->GetCount()));
            (*it).second.back()->GetUIText()->SetPosition(D3DXVECTOR3(130.0f, 0.0f,0.0f));

            //UIText test = *(static_cast<UIText*>(m_Text->GetChild(i)));
            //static_cast<UIText*>(m_Text->GetChild(i))->SetText(string(ItemInfo::GetName((*it).first))
            //    + "  " + to_string((*it).second.back()->GetCount()));
           /* a = new UIText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
                D3DXVECTOR2(100.0f, 50.0f),
                string(ItemInfo::GetName((*it).first))
                + "  " + to_string((*it).second.back()->GetCount()),
                D3DCOLOR_XRGB(200, 200, 200),
                m_Border);
            a->SetDrawTextFormat(DT_LEFT);
            a->SetPosition(D3DXVECTOR3(310.0f, 120.0f + 50.0f * static_cast<float>(i), 0.0f));*/
            //Gun* g = it->second;
            //UIText::Create(Font::kInteractionMessageDescription, g->GetName(), D3DXVECTOR3(600.0f, 100.0f + 50.0f * static_cast<float>(i), 0.0f), D3DXVECTOR2(200.0f, 50.0f), this);
            //rects.emplace_back(RECT{});
            //SetRect(&rects.back(), 350, 100 + 50 * i, 550, 150 + 50 * i);
            ++i;
        }
    }


}

void Character::TotalInventory::Render()
{
    if (isOpened)
    {
        // draw
    }
}

void Character::TotalInventory::SetEquipUI()
{
        new UIButtonWithItem(
        D3DXVECTOR3(440.0f, 117.0f, 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_yes.png",
        m_Border,
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        "Helmat",
        D3DCOLOR_XRGB(255, 0, 0),
        m_pHand,
        TAG_UI_POSITION::A,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));

        new UIButtonWithItem(
        D3DXVECTOR3(440.0f, 248.0f, 0.0f),
        "./Resource/UI/Inventory/Basic/",
        "Equip_no.png",
        "Equip_mouseover.png",
        "Equip_yes.png",
        m_Border,
        Resource()()->GetFont(TAG_FONT::Invetory_Ground),
        "Helmat",
        D3DCOLOR_XRGB(255, 0, 0),
        m_pHand,
        TAG_UI_POSITION::A,
        std::bind(
            &Character::onMouse,
            pCharacter,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
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
            (m_totalInventory.m_mapInventory)[tag].push_back(item);
            (m_totalInventory.m_capacity) += ItemInfo::GetCapacity(tag);
            CurrentScene()()->RemoveObject(item);
            // TODO : send "delete item on field" to server
        }
    }
        break;

    case TAG_ITEM_CATEGORY::Armor:
        checkOriginItem(&m_totalInventory.m_pEquipArmor, item);
        break;

    case TAG_ITEM_CATEGORY::Back:
        checkOriginItem(&m_totalInventory.m_pEquipBack, item);
        break;

    case TAG_ITEM_CATEGORY::Head:
        checkOriginItem(&m_totalInventory.m_pEquipHead, item);
        break;

    case TAG_ITEM_CATEGORY::Rifle:
        //TODO: 무기창의 어느부분에 넣느냐에 따라서 달라짐
        //지금은 임시로 QBZ를 주무기, Kar98k를 보조무기로
        {
            //if (주무기 창이면)
            if(tag == TAG_RES_STATIC::QBZ)
            {
                checkOriginItem((Item**)&m_totalInventory.m_pWeaponPrimary, item);
            }
            else if(tag == TAG_RES_STATIC::Kar98k)
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
    //ShowTotalInventory();
}

void Character::createOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item)
{
    assert(map && item && "Character::CreateOrMergeItem(), argument is null.");

    TAG_RES_STATIC tag = item->GetTagResStatic();
    auto it = map->find(tag);
    int count = item->GetCount();

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
    m_totalInventory.m_capacity += count * ItemInfo::GetCapacity(tag);
}

void Character::checkOriginItem(Item** originItem, Item* newItem)
{
    assert(newItem && "Character::checkOriginItem(), argument is null.");
    if (*originItem)
    {
        (*originItem)->SetIsRenderEffectMesh(true);
        //TODO: 바닥에 떨군다 & 소지용량 변경
        //아래는 임시코드
        CurrentScene()()->Destroy(*originItem);
        checkOriginItem(originItem, newItem);
    }
    else
    {
        *originItem = newItem;
        m_totalInventory.m_capacity += ItemInfo::GetCapacity(newItem->GetTagResStatic());
        CurrentScene()()->RemoveObject(newItem);

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

    cout << "용량: " << m_totalInventory.m_capacity << endl;
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
    if (event == Event::DRAG && 
        button == MouseButton::LEFT && 
        pUIButtonWithItem->m_tagUIPosition == TAG_UI_POSITION::floor_0)
    {
    }

    if (event == Event::DRAG)
    {
    }
}
