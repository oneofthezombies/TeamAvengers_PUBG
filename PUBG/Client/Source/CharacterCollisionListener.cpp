#include "stdafx.h"
#include "CharacterCollisionListener.h"
#include "CharacterPart.h"

CharacterCollisionListener::CharacterCollisionListener(IObject* pOwner)
    : ICollisionListener(pOwner)
{
}

CharacterCollisionListener::~CharacterCollisionListener()
{
}

void CharacterCollisionListener::OnCollisionEnter(
    Collider* pPerpetrator, Collider* pVictim)
{
    auto pCharacterPart = static_cast<CharacterPart*>(pVictim->GetOwner());
    auto tag = pCharacterPart->GetTagColliderCharacterPart();
    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::HEAD:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::NECK:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::BREAST:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::WAIST:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_LEFT:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_RIGHT:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_LEFT:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_UPPER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_UPPER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT:
        {

        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT:
        {

        }
        break;
    }
}

void CharacterCollisionListener::OnCollisionExit(
    Collider* pPerpetrator, Collider* pVictim)
{
}

void CharacterCollisionListener::OnCollisionStay(
    Collider* pPerpetrator, Collider* pVictim)
{
}