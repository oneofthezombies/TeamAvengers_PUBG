#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::PutItemInInventory(Item* item)
{
    TAG_RES_STATIC tag = item->GetTagResStatic();
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(tag);

    auto it = m_mapInventory.find(tag);
    int bulletNum = 0;
    switch (category)
    {
    case TAG_ITEM_CATEGORY::Ammo:
    {
        bulletNum = item->GetNumBullet();
        if (it == m_mapInventory.end())
        {
            //총알이 없는 경우는 새로 생성한다
            m_mapInventory[tag].push_back(item);
            item->SetNumBullet(bulletNum);
        }
        else
        {
            //이미 인벤토리에 해당 총알이 있는 경우, 기존 총알 개수와 합친다
            auto origin_item = (*it).second.back();
            origin_item->SetNumBullet(origin_item->GetNumBullet() + bulletNum);
        }
        m_capacity += bulletNum * ItemInfo::GetCapacity(tag);
    }
        break;

    case TAG_ITEM_CATEGORY::Attach:
    case TAG_ITEM_CATEGORY::Consumable:
        m_mapInventory[tag].push_back(item);
        m_capacity += ItemInfo::GetCapacity(tag);
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
}

void Character::ShowInventory()
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
            if (tag == TAG_RES_STATIC::Ammo_5_56mm ||
                tag == TAG_RES_STATIC::Ammo_7_62mm)
                cout << " " << item->GetNumBullet();
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