#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"

const float Character::TotalInventory::DEFAULT_CAPACITY = 70.0f;

Character::TotalInventory::TotalInventory()
    :m_pHand(nullptr)
    ,m_pEquipArmor(nullptr)
    ,m_pEquipBack(nullptr)
    ,m_pEquipHead(nullptr)
    ,m_pWeaponPrimary(nullptr)
    ,m_pWeaponSecondary(nullptr)
    ,m_capacity(DEFAULT_CAPACITY)
    ,m_numReload(0)
    ,m_bulletFireCoolDown(0)
    ,pTempSaveWeaponForX(nullptr)
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
        checkOriginItem(&m_totalInventory.m_pEquipArmor, item);
        break;

    case TAG_ITEM_CATEGORY::Back:
        checkOriginItem(&m_totalInventory.m_pEquipBack, item);
        break;

    case TAG_ITEM_CATEGORY::Head:
        checkOriginItem(&m_totalInventory.m_pEquipHead, item);
        break;

    case TAG_ITEM_CATEGORY::Rifle:
        //TODO: ����â�� ����κп� �ִ��Ŀ� ���� �޶���
        //������ �ӽ÷� QBZ�� �ֹ���, Kar98k�� ���������
    {
        //if (�ֹ��� â�̸�)
        if(tag == TAG_RES_STATIC::QBZ)
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

    Item* pHand = m_totalInventory.m_pHand;
    if (pHand)
    {
        Transform* pHandTr = pHand->GetTransform();
        D3DXMATRIX weaponWorld = 
            m_framePtr.pHandGun->CombinedTransformationMatrix
            * pTr->GetTransformationMatrix();

        pHandTr->SetTransformationMatrix(weaponWorld);
        pHandTr->Update();
    }

    Item* pArmor = m_totalInventory.m_pEquipArmor;
    if (pArmor)
    {
    }

    Item* pBack = m_totalInventory.m_pEquipBack;
    if (pBack)
    {

    }

    Item* pHead = m_totalInventory.m_pEquipHead;
    if (pHead)
    {

    }

    //�ֹ���
    Item* pWeaponPrimary = m_totalInventory.m_pWeaponPrimary;
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

    if (m_totalInventory.m_pEquipArmor)
    {
        tag = m_totalInventory.m_pEquipArmor->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pEquipArmor->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_pEquipBack)
    {
        tag = m_totalInventory.m_pEquipBack->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pEquipBack->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_pEquipHead)
    {
        tag = m_totalInventory.m_pEquipHead->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pEquipHead->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    cout << "<����>" << endl;

    if (m_totalInventory.m_pWeaponPrimary)
    {
        tag = m_totalInventory.m_pWeaponPrimary->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pWeaponPrimary->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }

    if (m_totalInventory.m_pWeaponSecondary)
    {
        tag = m_totalInventory.m_pWeaponSecondary->GetTagResStatic();
        category = ItemInfo::GetItemCategory(tag);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        cout << "- " << ItemInfo::GetName(tag);
        cout << " " << m_totalInventory.m_pWeaponSecondary->GetCount() << "��";
        cout << " �뷮: " << ItemInfo::GetCapacity(tag) << endl;
    }
}