#include "stdafx.h"
#include "Interpolation.h"

float Interpolation::GetMovingFactor(const TAG_ANIM_CHARACTER tag)
{
    switch (tag)
    {
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_R:
        return 180.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_R:
        return 260.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_R:
        return 120.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_R:
        return 180.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_R:
        return 120.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_R:
        return 180.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_R:
        return 260.0f;

    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_R:
        return 120.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_B:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_R:
        return 120.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponDown:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponInView:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponUp:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_BigGun:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponDown:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponUp:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_BigGun:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponDown:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponUp:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_L_WeaponDown:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_L_WeaponUp:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_R_WeaponDown:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_R_WeaponUp:
        return 200.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_B:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_R:
        return 100.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_B:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_R:
        return 120.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_R:
        return 100.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_B:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_R:
        return 120.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_R:
        return 200.0f;

    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_B:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_FL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_R:
        return 100.0f;

    default:
        return 120.0f;
    }
}
