#include "stdafx.h"
#include "Character.h"
#include "CharacterPart.h"
#include "Collider.h"

void Character::OnCollisionEnter(Collider* pOffence, Collider* pDefence)
{
    cout << "OnCollisionEnter\n";

    CharacterPart* pCharacterPart = 
        static_cast<CharacterPart*>(pDefence->GetOwner());

    TAG_COLLIDER_CHARACTER_PART tag = 
        pCharacterPart->GetTagColliderCharacterPart();

    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::Head:
        {
            cout << "head\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Neck:
        {
            cout << "neck\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Breast:
        {
            cout << "breast\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Upper:
        {
            cout << "stomach upper\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Lower:
        {
            cout << "stomach lower\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Waist:
        {
            cout << "waist\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Left:
        {
            cout << "clavicle left\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Right:
        {
            cout << "clavicle right\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Upper:
        {
            cout << "arm left upper\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Upper:
        {
            cout << "arm right upper\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Lower:
        {
            cout << "arm left lower\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Lower:
        {
            cout << "arm right upper\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Left:
        {
            cout << "hand left\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Right:
        {
            cout << "hand right\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Upper:
        {
            cout << "leg left upper\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Upper:
        {
            cout << "leg right upper\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Lower:
        {
            cout << "leg left lower\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Lower:
        {
            cout << "leg right lower\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Left:
        {
            cout << "foot left\n";
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Right:
        {
            cout << "foot right\n";
        }
        break;
    }
}

void Character::OnCollisionStay(Collider* pOffence, Collider* pDefence)
{
    Debug << "OnCollisionStay\n";
}

void Character::OnCollisionExit(Collider* pOffence, Collider* pDefence)
{
    cout << "OnCollisionExit\n";
}
