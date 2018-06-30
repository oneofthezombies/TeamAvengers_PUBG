#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::PutItemInTotalInventory(Item* item)
{
    TAG_RES_STATIC tag = item->GetTagResStatic();
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(tag);

    switch (category)
    {
    case TAG_ITEM_CATEGORY::Ammo:
        CreateOrMergeItem(&m_mapInventory, item);
        break;

    case TAG_ITEM_CATEGORY::Attach:
    case TAG_ITEM_CATEGORY::Consumable:
    {
        if (tag == TAG_RES_STATIC::Bandage ||
            tag == TAG_RES_STATIC::FirstAidKit)
        {
            CreateOrMergeItem(&m_mapInventory, item);
        }
        else
        {
            m_mapInventory[tag].push_back(item);
            m_capacity += ItemInfo::GetCapacity(tag);
        }
    }
        break;

    case TAG_ITEM_CATEGORY::Equipment:
        m_mapEquip[tag].push_back(item);
        m_capacity += ItemInfo::GetCapacity(tag);
        break;

    case TAG_ITEM_CATEGORY::Weapon:
        m_mapWeapon[tag].push_back(item);
        m_capacity += ItemInfo::GetCapacity(tag);
        break;
    }
   
    item->SetIsInField(false);
}

void Character::CreateOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item)
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
    }
    else
    {
        //이미 인벤토리에 붕대가 있는 경우, 기존 붕대 개수와 합친다
        auto origin_item = it->second.back();
        origin_item->SetCount(origin_item->GetCount() + count);
        SAFE_DELETE(item);
    }
    m_capacity += count * ItemInfo::GetCapacity(tag);
}

void Character::ShowTotalInventory()
{
    cout << "용량: " << m_capacity << endl;
    cout << "<인벤토리>" << endl;
    for (auto items : m_mapInventory)
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        for (auto item : items.second)
        {
            auto tag = item->GetTagResStatic();
            cout << "- " << ItemInfo::GetName(tag);
            cout << " " << item->GetCount() << "개";
            cout << " 용량: " << ItemInfo::GetCapacity(tag);
            cout << endl;
        }
    }

    cout << "<장비>" << endl;
    for (auto items : m_mapEquip)
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        for (auto item : items.second)
        {
            auto tag = item->GetTagResStatic();
            cout << "- " << ItemInfo::GetName(tag);
            cout << " " << item->GetCount();
            cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
        }
    }

    cout << "<무기>" << endl;
    for (auto items : m_mapWeapon)
    {
        TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(items.first);
        cout << "[" << ForDebugGetItemCategory(category) << "] ";
        for (auto item : items.second)
        {
            auto tag = item->GetTagResStatic();
            cout << "- " << ItemInfo::GetName(tag);
            cout << " " << item->GetCount();
            cout << " 용량: " << ItemInfo::GetCapacity(tag) << endl;
        }
    }
}

string Character::ForDebugGetItemCategory(TAG_ITEM_CATEGORY category)
{
    switch (category)
    {
    case TAG_ITEM_CATEGORY::Equipment: return "Equipment";
    case TAG_ITEM_CATEGORY::Consumable: return "Consumable";
    case TAG_ITEM_CATEGORY::Ammo: return "Ammo";
    case TAG_ITEM_CATEGORY::Weapon: return "Weapon";
    case TAG_ITEM_CATEGORY::Attach: return "Attach";
    default: return "";
    }
}