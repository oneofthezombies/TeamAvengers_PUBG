#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"

bool Character::PutItemInInventory(Item* item)
{
    TAG_RES_STATIC tag = item->GetTagResStatic();
    TAG_ITEM_CATEGORY category = ItemInfo::GetItemCategory(tag);

    m_mapInventory[category].push_back(item);
    return true;
}

void Character::ShowInventory()
{
    for (auto items : m_mapInventory)
    {
        cout << ForDebugGetItemCategory(items.first) << endl;
        for (auto item : items.second)
        {
            cout << "- " << ItemInfo::GetName(item->GetTagResStatic()) << endl;
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