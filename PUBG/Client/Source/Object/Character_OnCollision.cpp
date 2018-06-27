#include "stdafx.h"
#include "Character.h"
#include "CharacterPart.h"
#include "Collider.h"

void Character::OnCollisionEnter(Collider* pPerpetrator, Collider* pVictim)
{
    cout << "OnCollisionEnter\n";

    auto pCharacterPart = static_cast<CharacterPart*>(pVictim->GetOwner());
    auto tag = pCharacterPart->GetTagColliderCharacterPart();
    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::Head:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Neck:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Breast:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Upper:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Lower:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Waist:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Left:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Right:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Upper:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Upper:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Lower:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Lower:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Left:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Right:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Upper:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Upper:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Lower:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Lower:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Left:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Right:
        {

        }
        break;
    }
}

void Character::OnCollisionStay(Collider* pPerpetrator, Collider* pVictim)
{
    Debug << "OnCollisionStay\n";
}

void Character::OnCollisionExit(Collider* pPerpetrator, Collider* pVictim)
{
    cout << "OnCollisionExit\n";
}
