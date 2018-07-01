#include "stdafx.h"
#include "ItemInfo.h"

TAG_ITEM_CATEGORY ItemInfo::GetItemCategory(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
        //Ammunition
    case TAG_RES_STATIC::Ammo_5_56mm: return TAG_ITEM_CATEGORY::Ammo;
    case TAG_RES_STATIC::Ammo_7_62mm: return TAG_ITEM_CATEGORY::Ammo;

        //Attachment
    case TAG_RES_STATIC::ACOG:       return TAG_ITEM_CATEGORY::Attach;
    case TAG_RES_STATIC::Aimpoint2X: return TAG_ITEM_CATEGORY::Attach;
    case TAG_RES_STATIC::RedDot:     return TAG_ITEM_CATEGORY::Attach;

        //Consumable
    case TAG_RES_STATIC::Bandage:     return TAG_ITEM_CATEGORY::Consumable;
    case TAG_RES_STATIC::FirstAidKit: return TAG_ITEM_CATEGORY::Consumable;
    case TAG_RES_STATIC::MedKit:      return TAG_ITEM_CATEGORY::Consumable;

        //Equipment
    case TAG_RES_STATIC::Armor_Lv1: return TAG_ITEM_CATEGORY::Armor;
    case TAG_RES_STATIC::Back_Lv1:  return TAG_ITEM_CATEGORY::Back;
    case TAG_RES_STATIC::Head_Lv1:  return TAG_ITEM_CATEGORY::Head;

        //Weapon
    case TAG_RES_STATIC::QBZ:    return TAG_ITEM_CATEGORY::Rifle;
    case TAG_RES_STATIC::Kar98k: return TAG_ITEM_CATEGORY::Rifle;

    default: 
        assert(false && "ItemInfo::GetItemCategory(), default case.");
        return TAG_ITEM_CATEGORY::Idle;
    }
}

string ItemInfo::GetName(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
        //Ammunition
    case TAG_RES_STATIC::Ammo_5_56mm: return "5.56mm";
    case TAG_RES_STATIC::Ammo_7_62mm: return "7.62mm";

        //Attachment
    case TAG_RES_STATIC::ACOG: return "ACOG";
    case TAG_RES_STATIC::Aimpoint2X: return "2X Aimpoint Scope";
    case TAG_RES_STATIC::RedDot: return "Red Dot Sight";

        //Consumable
    case TAG_RES_STATIC::Bandage: return "Bandage";
    case TAG_RES_STATIC::FirstAidKit: return "First Aid Kit";
    case TAG_RES_STATIC::MedKit: return "Med Kit";

        //Equipment
    case TAG_RES_STATIC::Armor_Lv1: return "Police Vest (Level 1)";
    case TAG_RES_STATIC::Back_Lv1: return  "Backpack (Level 1)";
    case TAG_RES_STATIC::Head_Lv1: return  "Motocycle Helmet (Level 1)";

        //Weapon
    case TAG_RES_STATIC::QBZ: return "OBZ";
    case TAG_RES_STATIC::Kar98k: return "Kar98k";

    default: return "";
    }
}

string ItemInfo::GetDescription(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
        //Ammunition
    case TAG_RES_STATIC::Ammo_5_56mm: return "Ammo for M16A4, M416, SCAR - L, AUG, QBZ95, Mini14, M249";
    case TAG_RES_STATIC::Ammo_7_62mm: return "Ammo for AKM, Groza, Kar98k, M24, SKS, Mk14, DP-28, SLR, R1895";

        //Attachment
    case TAG_RES_STATIC::ACOG: return "4.00x Magnification";
    case TAG_RES_STATIC::Aimpoint2X: return " 1.80x Magnification +10.00% Faster ADS";
    case TAG_RES_STATIC::RedDot: return "+ 20.00% Faster ADS";

        //Consumable
    case TAG_RES_STATIC::Bandage: return "Bandages heal a character's health by 10 overtime. Performing certain actions while casting this item will cancel it. It cannot heal character's health over 75.";
    case TAG_RES_STATIC::FirstAidKit: return "First Aid Kits heal a character's health to 75 instantly. Performing certain actions while casting this item will cancel it. This item cannot be used when player's health is over 75.";
    case TAG_RES_STATIC::MedKit: return "Med Kits heal a character's health to 100 instantly. Performing certain actions while casting this item will cancel it.";

        //Equipment
    case TAG_RES_STATIC::Armor_Lv1: return "Capacity + 50";
    case TAG_RES_STATIC::Back_Lv1: return "Capacity + 150";
    case TAG_RES_STATIC::Head_Lv1: return "The Jacob Helmet";

        //Weapon
    case TAG_RES_STATIC::QBZ: return "Modern bullpup assault rifle";
    case TAG_RES_STATIC::Kar98k: return "Vintage sniper rifle";

    default: return "";
    }
}

float ItemInfo::GetCapacity(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
        //Ammunition
    case TAG_RES_STATIC::Ammo_5_56mm: return 0.5f;
    case TAG_RES_STATIC::Ammo_7_62mm: return 0.7f;

        //Attachment
    case TAG_RES_STATIC::ACOG: return 15.0f;
    case TAG_RES_STATIC::Aimpoint2X: return 15.0f;
    case TAG_RES_STATIC::RedDot: return 10.0f;

        //Consumable
    case TAG_RES_STATIC::Bandage: return 2.0f;
    case TAG_RES_STATIC::FirstAidKit: return 10.0f;
    case TAG_RES_STATIC::MedKit: return 20.0f;

        //Equipment
    case TAG_RES_STATIC::Armor_Lv1: return 70.0f;
    case TAG_RES_STATIC::Back_Lv1: return 400.f;
    case TAG_RES_STATIC::Head_Lv1: return 80.0f;

        //Weapon
    case TAG_RES_STATIC::QBZ: return 500.f;
    case TAG_RES_STATIC::Kar98k: return 500.f;

    default: return 0.0f;
    }
}

float ItemInfo::GetFasterADS(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::RedDot: return 1.2f;
    case TAG_RES_STATIC::Aimpoint2X: return 1.1f;

    default: return 1.0f;
    }
}

float ItemInfo::GetMagnification(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Aimpoint2X: return 1.8f;
    case TAG_RES_STATIC::ACOG: return 4.0f;

    default:return  1.0f;
    }
}

float ItemInfo::GetDamageReduction(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Head_Lv1: return 0.7f;
    case TAG_RES_STATIC::Armor_Lv1: return 0.7f;

    default: return 1.0f;
    }
}

float ItemInfo::GetDefaultDurability(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Head_Lv1: return 80.0f;
    case TAG_RES_STATIC::Armor_Lv1: return 200.0f;

    default: return 0.0f;
    }
}

float ItemInfo::GetCapacityExtension(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Armor_Lv1: return 50.0f;
    case TAG_RES_STATIC::Back_Lv1: return 150.0f;

    default: return 0.0f;
    }
}

float ItemInfo::GetCastTime(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Bandage: return 4.0f;
    case TAG_RES_STATIC::FirstAidKit: return 6.0f;
    case TAG_RES_STATIC::MedKit: return 8.0f;

    default: return 0.0f;
    }
}

float ItemInfo::GetRecovery(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Bandage: return 10.0f;
    case TAG_RES_STATIC::FirstAidKit: return 75.0f;
    case TAG_RES_STATIC::MedKit: return 100.0f;

    default: return 0.0f;
    }
}
