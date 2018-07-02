#pragma once
#include "IObject.h"

class BoxCollider;

class SampleCollisionBox : public IObject
{
private:
    BoxCollider * pBC;
    D3DXVECTOR3 v;
public:
    SampleCollisionBox();
    virtual~SampleCollisionBox();
    // Inherited via IObject
    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void OnCollisionEnter(Collider* a, Collider* b);
    void OnCollisionStay(Collider* a, Collider* b);
    void OnCollisionExit(Collider* a, Collider* b);
};