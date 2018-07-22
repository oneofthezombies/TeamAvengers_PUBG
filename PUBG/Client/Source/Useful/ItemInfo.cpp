#include "stdafx.h"
#include "ItemInfo.h"
#include "TagClientOnly.h"

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
    case TAG_RES_STATIC::Armor_Lv1: return "Vest (Lv 1)";
    case TAG_RES_STATIC::Back_Lv1: return  "Backpack (Lv 1)";
    case TAG_RES_STATIC::Head_Lv1: return  "Helmet (Lv 1)";

        //Weapon
    case TAG_RES_STATIC::QBZ: return "QBZ";
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

    default: return  1.0f;
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

float ItemInfo::GetMagazineSize(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ: return 30.0f;
    case TAG_RES_STATIC::Kar98k: return 5.0f;

    default: return 0.0f;
    }
}

TAG_RES_STATIC ItemInfo::GetAmmoType(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ: return TAG_RES_STATIC::Ammo_5_56mm;
    case TAG_RES_STATIC::Kar98k: return TAG_RES_STATIC::Ammo_7_62mm;

    default: 
        assert(false && "ItemInfo::GetAmmoType(), default case.");
        return TAG_RES_STATIC::COUNT;
    }
}

float ItemInfo::GetBaseDamage(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ: return 43.0f;
    case TAG_RES_STATIC::Kar98k: return 75.0f;

    default: return 0.0f;
    }
}

float ItemInfo::GetInitialBulletSpeed(const TAG_RES_STATIC tag)
{
    //단위는 m/s
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ: return 8700.0f;/*87000.0f;*/
    case TAG_RES_STATIC::Kar98k: return 7600.0f;/*76000.0f;*/

    default: return 0.0f;
    }
}

float ItemInfo::GetDropOffByDistance(const float distance, const TAG_RES_STATIC tag)
{
    //CM
    float maxDist = std::numeric_limits<float>::max();
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ:    maxDist = 20000.0f; break;
    case TAG_RES_STATIC::Kar98k: maxDist = 40000.0f; break;
    default:
        {
            assert(false && "ItemInfo::GetDropOffByDistance(), default case.");
            break;
        }
    }
    
    float dropOffPercentage = 1.0f - 0.25f * distance / maxDist;
    if (dropOffPercentage < 0.0f)
        dropOffPercentage = 0.0f;
    return dropOffPercentage;
}

float ItemInfo::GetBulletFireCoolTime(const TAG_RES_STATIC tag)
{
    //단위는 sec
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ: return 0.092f;
    case TAG_RES_STATIC::Kar98k: return 1.900f;

    default: return 0.0f;
    }
}

float CharacterInfo::GetWeaponClassDamageByHitZone(const TAG_COLLIDER_CHARACTER_PART tag)
{
    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::Head:
    case TAG_COLLIDER_CHARACTER_PART::Neck:
        return 2.30f;
    case TAG_COLLIDER_CHARACTER_PART::Breast:
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Waist:
        return 1.0f;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Left:
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Right:
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Hand_Left:
    case TAG_COLLIDER_CHARACTER_PART::Hand_Right:
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Foot_Left:
    case TAG_COLLIDER_CHARACTER_PART::Foot_Right:
        return 0.9f;
    default:
        assert(false && "GetWeaponClassDamageByHitZone(), default case.");
        return 1.0f;
    }
}

float CharacterInfo::GetHitAreaDamage(const TAG_COLLIDER_CHARACTER_PART tag)
{
    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::Head:
        return 1.0f;
    case TAG_COLLIDER_CHARACTER_PART::Neck:
        return 0.75f;
    case TAG_COLLIDER_CHARACTER_PART::Breast:
        return 1.1f;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Upper:
        return 1.0f;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Lower:
        return 0.9f;
    case TAG_COLLIDER_CHARACTER_PART::Waist:
        return 1.0f;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Left:
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Right:
        return 1.0f;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Upper:
        return 0.6f;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Lower:
        return 0.5f;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Left:
    case TAG_COLLIDER_CHARACTER_PART::Hand_Right:
        return 0.3f;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Upper:
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Upper:
        return 0.5f;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Lower:
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Lower:
        return 0.5f;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Left:
    case TAG_COLLIDER_CHARACTER_PART::Foot_Right:
        return 0.3f;
    default:
        assert(false && "CharacterInfo::GetHitAreaDamage, default case.");
        return 1.0f;
    }

}
