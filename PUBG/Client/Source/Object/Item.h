#pragma once
#include "IObject.h"

class EffectMeshRenderer;

class Item : public IObject
{
private:
    const TAG_RES_STATIC m_tagResStatic;
    bool  m_isInField;
    float m_durability;
    int   m_numBullet;

    EffectMeshRenderer* pEffectMeshRenderer;

private:
    void setup(const TAG_RES_STATIC tag);

public:
             Item(
                 const TAG_RES_STATIC tag, 
                 const D3DXVECTOR3& position, 
                 const D3DXVECTOR3& rotation, 
                 const D3DXVECTOR3& scale);
    virtual ~Item();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    TAG_RES_STATIC GetTagResStatic() const;
};