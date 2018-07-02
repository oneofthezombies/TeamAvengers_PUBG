#include "stdafx.h"
#include "Character.h"

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

string Character::ForDebugGetAttacking(Attacking attcking)
{
    switch (attcking)
    {
    case Attacking::Unarmed: return "Unarmed";
    case Attacking::Rifle:   return "Rifle";
    case Attacking::Melee:   return "Melee";
    default:
        assert(false && "Character::ForDebugGetAttacking(), default case.");
        return "";
    }
}