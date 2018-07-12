#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"

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

string Character::ForDebugGetStance(Stance stance)
{
    switch (stance)
    {
    case Stance::Stand: return "Stand";
    case Stance::Crouch: return "Crouch";
    case Stance::Prone: return "Prone";
    default:
        assert(false && "Character::ForDebugGetStance(), default case.");
        return "";   
    }
}

string Character::ForDebugGetMoving(Moving moving)
{
    switch (moving)
    {
    case Moving::Run: return "Run";
    case Moving::Sprint: return "Sprint";
    case Moving::Walk: return "Walk";
    default:
        assert(false && "Character::ForDebugGetMoving(), default case.");
        return "";
    }
}

void Character::ForDebug()
{
    //카메라 모드
    TAG_CAMERA tagCamera = CurrentCamera()()->GetTagCamera();
    switch (tagCamera)
    {
    case TAG_CAMERA::First_Person:
        Debug << "Camera Mode: " << "First Person" << "\n";
        break;

    case TAG_CAMERA::Third_Person:
        Debug << "Camera Mode: " << "Third Person" << "\n";
        break;

    case TAG_CAMERA::Default:
        Debug << "Camera Mode: " << "Default" << "\n";
        break;

    case TAG_CAMERA::KyunChak:
        Debug << "Camera Mode: " << "KyunChak" << "\n";
        break;

    case TAG_CAMERA::Scope2X:
        Debug << "Camera Mode: " << "Scope2X" << "\n";
        break;

    default:
        Debug << "Camera Mode: " << "NONE" << "\n";
        break;
    }

    Item* hand = m_totalInventory.m_pHand;
    Item* saveWeapon = m_totalInventory.pTempSaveWeaponForX;
    if (hand)
    {
        TAG_RES_STATIC tagDebug = hand->GetTagResStatic();
        Debug << "On hand Weapon: ";
        Debug << ItemInfo::GetName(tagDebug) << "\n";
    }
    else
    {
        Debug << "On hand Weapon: NONE" << "\n";
    }

    if (saveWeapon)
    {
        TAG_RES_STATIC tagDebug = saveWeapon->GetTagResStatic();
        Debug << "Temp save Weapon for X: ";
        Debug << ItemInfo::GetName(tagDebug) << "\n";   
    }
    else
    {
        Debug << "Temp save Weapon for X: NONE" << "\n";
    }

    Debug << "Attacking: " << ForDebugGetAttacking(m_attacking) << "\n";
    Debug << "Stance: "    << ForDebugGetStance(m_stance)       << "\n";
    Debug << "Moving: "    << ForDebugGetMoving(m_moving)       << "\n";

    if (m_Jump.isJumping)
        Debug << "IsJump: TRUE"  << "\n";
    else
        Debug << "IsJump: FALSE" << "\n";
}