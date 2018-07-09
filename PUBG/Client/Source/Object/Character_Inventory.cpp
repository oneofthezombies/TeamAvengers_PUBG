#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
///////�߰��Ѱ� UI
#include "UIImage.h"
#include "UIText.h"

const float Character::TotalInventory::DEFAULT_CAPACITY = 70.0f;

Character::TotalInventory::TotalInventory()
    : m_hand(nullptr)
    , m_equipArmor(nullptr)
    , m_equipBack(nullptr)
    , m_equipHead(nullptr)
    , m_weaponPrimary(nullptr)
    , m_weaponSecondary(nullptr)
    , m_capacity(DEFAULT_CAPACITY)
    , m_numReload(0)
    , m_bulletFireCoolDown(0)
    , m_tempSaveWeaponForX(nullptr)
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

    float miniPer = 0.67f; //miniPersentage
    auto b = new UIImage(
        "./Resource/UI/Inventory/Character/",
        "female.tga",
        D3DXVECTOR3(410.0f / miniPer, 85.0f / miniPer, 0.0f),
        nullptr,
        m_Border);

    //������ �̹��� size ����
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
    SAFE_DELETE(m_hand);

    for (auto v : m_mapInventory)
        for (auto i : v.second)
            SAFE_DELETE(i);

    SAFE_DELETE(m_equipArmor);
    SAFE_DELETE(m_equipBack);
    SAFE_DELETE(m_equipHead);

    SAFE_DELETE(m_weaponPrimary);
    SAFE_DELETE(m_weaponSecondary);

    SAFE_DELETE(m_tempSaveWeaponForX);
}


void Character::TotalInventory::Open()
{
    isOpened = true;
    // move ui to in screen

    m_Border->SetIsRender(isOpened);
    
    for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); it++)
    {
        (*it).second.back()->SetInRenderUIImage(isOpened);
    }

    if (m_Text->GetChild(0)==nullptr)
    {

        vector<RECT> rects;
        int i = 0;
        for (auto it = m_mapInventory.begin(); it != m_mapInventory.end(); it++)
        {
            (*it).second.back()->SetUIPosition(D3DXVECTOR2(250.0f / 0.2f, (100.0f + 50.0f * static_cast<float>(i)) / 0.2f));
            auto c = new UIText(Resource()()->GetFont(TAG_FONT::Invetory_Ground),
                D3DXVECTOR2(100.0f, 50.0f),
                string(ItemInfo::GetName((*it).first))
                + "  " + to_string((*it).second.back()->GetCount()),
                D3DCOLOR_XRGB(200, 200, 200),
                m_Text);
            c->SetDrawTextFormat(DT_LEFT);
            c->SetPosition(D3DXVECTOR3(310.0f, 120.0f + 50.0f * static_cast<float>(i), 0.0f));
            //Gun* g = it->second;
            //UIText::Create(Font::kInteractionMessageDescription, g->GetName(), D3DXVECTOR3(600.0f, 100.0f + 50.0f * static_cast<float>(i), 0.0f), D3DXVECTOR2(200.0f, 50.0f), this);
            rects.emplace_back(RECT{});
            SetRect(&rects.back(), 350, 100 + 50 * i, 550, 150 + 50 * i);
            ++i;
        }
    }
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
            UIText test = *(static_cast<UIText*>(m_Text->GetChild(i)));
            static_cast<UIText*>(m_Text->GetChild(i))->SetText(string(ItemInfo::GetName((*it).first))
                + "  " + to_string((*it).second.back()->GetCount()));
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

void Character::PutItemInTotalInventory(Item* item)
{
    assert(item && "Character::PutItemInTotalInventory(), item is null.");

    TAG_RES_STATIC tag = item->GetTagResStatic();
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
        checkOriginItem(&m_totalInventory.m_equipArmor, item);
        break;

    case TAG_ITEM_CATEGORY::Back:
        checkOriginItem(&m_totalInventory.m_equipBack, item);
        break;

    case TAG_ITEM_CATEGORY::Head:
        checkOriginItem(&m_totalInventory.m_equipHead, item);
        break;

    case TAG_ITEM_CATEGORY::Rifle:
        //TODO: ����â�� ����κп� �ִ��Ŀ� ���� �޶���
        //������ �ӽ÷� QBZ�� �ֹ���, Kar98k�� ���������
    {
        //if (�ֹ��� â�̸�)
        if(tag == TAG_RES_STATIC::QBZ)
        {
            checkOriginItem((Item**)&m_totalInventory.m_weaponPrimary, item);
        }
        else
        {
            checkOriginItem((Item**)&m_totalInventory.m_weaponSecondary, item);
        }
    }
        break;

    default:
        assert(false && "PutItemInTotalInventory(), default case.");
        break;
    }
}

void Character::createOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item)
{
    assert(map && item && "Character::CreateOrMergeItem(), argument is null.");

    TAG_RES_STATIC tag = item->GetTagResStatic();
    auto it = map->find(tag);
    int count = item->GetCount();

    if (it == map->end())
    {
       //�������� ���� ���� ���� �����Ѵ�
        (*map)[tag].push_back(item);
        item->SetCount(count);
        CurrentScene()()->RemoveObject(item);
        // TODO : send "delete item on field" to server
    }
    else
    {
        //�̹� �κ��丮�� �ش밡 �ִ� ���, ���� �ش� ������ ��ģ��
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
        //TODO: �ٴڿ� ������ & �����뷮 ����
        //�Ʒ��� �ӽ��ڵ�
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

    Item* pHand = m_totalInventory.m_hand;
    if (pHand)
    {
        Transform* pHandTr = pHand->GetTransform();
        D3DXMATRIX weaponWorld = 
            m_framePtr.pHandGun->CombinedTransformationMatrix
            * pTr->GetTransformationMatrix();

        pHandTr->SetTransformationMatrix(weaponWorld);
        pHandTr->Update();
    }

    Item* pArmor = m_totalInventory.m_equipArmor;
    if (pArmor)
    {
    }

    Item* pBack = m_totalInventory.m_equipBack;
    if (pBack)
    {

    }

    Item* pHead = m_totalInventory.m_equipHead;
    if (pHead)
    {

    }

    //�ֹ���
    Item* pWeaponPrimary = m_totalInventory.m_weaponPrimary;
    if (pWeaponPrimary)
    {
        Transform* pWeaponPrimaryTr = pWeaponPrimary->GetTransform();

        // �𵨽����̽� m_framePtr.pSlotPrimary->CombinedTransformationMatrix
        // ���彺���̽� = �𵨽����̽� * ĳ���� ���彺���̽�

        D3DXMATRIX weaponWorld = 
            m_framePtr.pSlotPrimary->CombinedTransformationMatrix
            * pTr->GetTransformationMatrix();

        pWeaponPrimaryTr->SetTransformationMatrix(weaponWorld);

        //Debug << "primary weapon matrix : \n" << weaponWorld << '\n';

        pWeaponPrimaryTr->Update();
    }
    
    //��������
    Item* pWeaponSecondary = m_totalInventory.m_weaponSecondary;
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
    Item* pHand = m_totalInventory.m_hand;
    if (pHand) pHand->Render();

    Item* pArmor = m_totalInventory.m_equipArmor;
    if (pArmor) pArmor->Render();

    Item* pBack = m_totalInventory.m_equipBack;
    if (pBack) pBack->Render();

    Item* pHead = m_totalInventory.m_equipHead;
    if (pHead) pHead->Render();

    Item* pWeaponPrimary = m_totalInventory.m_weaponPrimary;
    if (pWeaponPrimary) pWeaponPrimary->Render();

    Item* pWeaponSecondary = m_totalInventory.m_weaponSecondary;
    if (pWeaponSecondary) pWeaponSecondary->Render();
}

void Character::ShowTotalInventory()
{
    TAG_RES_STATIC tag;

    cout << "�뷮: " << m_totalInventory.m_capacity << endl;
    cout << "<�κ��丮>" << endl;
    for (auto items : m_totalInventory.m_mapInventory)
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        for (auto item : items.second)
        {
            tag = item->GetTagResStatic();
            cout << "- " << ItemInfo::GetName(tag);
            cout << " " << item->GetCount() << "��";
            cout << " �뷮: " << ItemInfo::GetCapacity(tag);
            cout << endl;
        }
    }

    TAG_ITEM_CATEGORY category;
    cout << "<���>" << endl;

    if (m_totalInventory.m_equipArmor)
    {
        tag = m_totalInventory.m_equipArmor->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_equipArmor->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_equipBack)
    {
        tag = m_totalInventory.m_equipBack->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_equipBack->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_equipHead)
    {
        tag = m_totalInventory.m_equipHead->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_equipHead->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    cout << "<����>" << endl;

    if (m_totalInventory.m_weaponPrimary)
    {
        tag = m_totalInventory.m_weaponPrimary->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_weaponPrimary->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_weaponSecondary)
    {
        tag = m_totalInventory.m_weaponSecondary->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_weaponSecondary->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }
}