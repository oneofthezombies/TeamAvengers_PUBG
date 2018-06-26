#pragma once
#include "IObject.h"
#include "TagResource.h"

class BoxCollider;
class EffectMeshRenderer;

class CollidableStaticObject : public IObject
{
private:
    vector<BoxCollider*> m_colliders;
    EffectMeshRenderer*  pEffectMeshRenderer;

public:
             CollidableStaticObject(
                 const TAG_RES_STATIC tag, 
                 const D3DXVECTOR3&   position,
                 const D3DXVECTOR3&   rotation,
                 const D3DXVECTOR3&   scale);

    virtual ~CollidableStaticObject();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void AddBoxCollider(
        const D3DXVECTOR3& center, 
        const D3DXVECTOR3& extent, 
        const D3DXMATRIX&  transform);
};