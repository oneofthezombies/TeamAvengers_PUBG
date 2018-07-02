#include "stdafx.h"
#include "Character.h"

void Character::onKar98kReloadEnd()
{
    setAnimation(TAG_ANIM_CHARACTER::Kar98k_Reload_End, false, [this]()
    {
        setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);
    });
}

void Character::onKar98kReload()
{
    if (m_totalInventory.m_numReload == 1)
    {
        onKar98kReloadEnd();
    }
    else
    {
        setAnimation(TAG_ANIM_CHARACTER::Kar98k_Reload_Loop, false, std::bind(&Character::onKar98kReload, this));
    }

    --m_totalInventory.m_numReload;
}
