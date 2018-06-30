#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"

const float Character::TotalInventory::DEFAULT_CAPACITY = 70.0f;

Character::TotalInventory::TotalInventory()
    : m_equipArmor(nullptr)
    , m_equipBack(nullptr)
    , m_equipHead(nullptr)
    , m_weaponPrimary(nullptr)
    , m_weaponSecondary(nullptr)
    , m_capacity(DEFAULT_CAPACITY)
{
}

Character::TotalInventory::~TotalInventory()
{
    for (auto v : m_mapInventory)
        for (auto i : v.second)
            SAFE_DELETE(i);

    SAFE_DELETE(m_equipArmor);
    SAFE_DELETE(m_equipBack);
    SAFE_DELETE(m_equipHead);

    SAFE_DELETE(m_weaponPrimary);
    SAFE_DELETE(m_weaponSecondary);
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
        //TODO: 무기창의 어느부분에 넣느냐에 따라서 달라짐
        //지금은 임시로 QBZ를 주무기, Kar98k를 보조무기로
    {
        //if (주무기 창이면)
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
   
    item->SetIsRenderEffectMesh(false);
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
        //이미 인벤토리에 붕대가 있는 경우, 기존 붕대 개수와 합친다
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
        // TODO : send "delete item on field" to server
    }
}

void Character::updateTotalInventory()
{

}

void Character::renderTotalInventory()
{
    Item* pArmor = m_totalInventory.m_equipArmor;
    if (pArmor) pArmor->Render();

    Item* pBack = m_totalInventory.m_equipBack;
    if (pBack) pBack->Render();

    Item* pHead = m_totalInventory.m_equipHead;
    if (pHead) pHead->Render();

    Weapon* pWeaponPrimary = m_totalInventory.m_weaponPrimary;
    if (pWeaponPrimary) pWeaponPrimary->Render();

    Weapon* pWeaponSecondary = m_totalInventory.m_weaponSecondary;
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

    //cout << "<장비>" << endl;
    //for (auto items : m_totalInventory.m_mapEquip)
    //{
    //    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
    //    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    //    for (auto item : items.second)
    //    {
    //        tag = item->GetTagResStatic();
    //        cout << "- " << ItemInfo::GetName(tag);
    //        cout << " " << item->GetCount();
    //        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    //    }
    //}
    cout << "<장비>" << endl;
    tag = m_totalInventory.m_equipArmor->GetTagResStatic();
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(tag);
    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    cout << "- " << ItemInfo::GetName(tag);
    cout << " " << m_totalInventory.m_equipArmor->GetCount() << "개";
    cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;

    tag = m_totalInventory.m_equipBack->GetTagResStatic();
    category = ItemInfo::GetItemCategory(tag);
    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    cout << "- " << ItemInfo::GetName(tag);
    cout << " " << m_totalInventory.m_equipBack->GetCount() << "개";
    cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;

    tag = m_totalInventory.m_equipHead->GetTagResStatic();
    category = ItemInfo::GetItemCategory(tag);
    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    cout << "- " << ItemInfo::GetName(tag);
    cout << " " << m_totalInventory.m_equipHead->GetCount() << "개";
    cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;

    //cout << "<무기>" << endl;
    //for (auto items : m_mapWeapon)
    //{
    //    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
    //    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    //    for (auto item : items.second)
    //    {
    //        tag = item->GetTagResStatic();
    //        cout << "- " << ItemInfo::GetName(tag);
    //        cout << " " << item->GetCount();
    //        cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
    //    }
    //}

    cout << "<무기>" << endl;
    tag = m_totalInventory.m_weaponPrimary->GetTagResStatic();
    category = ItemInfo::GetItemCategory(tag);
    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    cout << "- " << ItemInfo::GetName(tag);
    cout << " " << m_totalInventory.m_weaponPrimary->GetCount() << "개";
    cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;

    tag = m_totalInventory.m_weaponSecondary->GetTagResStatic();
    category = ItemInfo::GetItemCategory(tag);
    cout << "[" << ForDebugGetItemCategory(category) << "] ";
    cout << "- " << ItemInfo::GetName(tag);
    cout << " " << m_totalInventory.m_weaponSecondary->GetCount() << "개";
    cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
}

string Character::ForDebugGetItemCategory(TAG_ITEM_CATEGORY category)
{
    switch (category)
    {
    case TAG_ITEM_CATEGORY::Armor: return "Armor";
    case TAG_ITEM_CATEGORY::Back: return "Backpack";
    case TAG_ITEM_CATEGORY::Head: return "Helmet";
    case TAG_ITEM_CATEGORY::Consumable: return "Consumable";
    case TAG_ITEM_CATEGORY::Ammo: return "Ammo";
    case TAG_ITEM_CATEGORY::Rifle: return "Rifle";
    case TAG_ITEM_CATEGORY::Attach: return "Attach";
    default: 
        assert(false && "Character::ForDebugGetItemCategory(), default case.");
        return "";
    }
}