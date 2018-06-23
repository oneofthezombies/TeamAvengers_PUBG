#pragma once
#include "Collider.h"

class CharacterCollisionListener : public ICollisionListener
{
public:
    CharacterCollisionListener(IObject* pOwner);
    virtual ~CharacterCollisionListener();

    virtual void OnCollisionEnter(
        Collider* pPerpetrator, Collider* pVictim) override;
    
    virtual void OnCollisionExit(
        Collider* pPerpetrator, Collider* pVictim) override;
    
    virtual void OnCollisionStay(
        Collider* pPerpetrator, Collider* pVictim) override;
};