#pragma once
#include "IObject.h"
#include "TagResource.h"

class BoxCollider;
class EffectMeshRenderer;

// e.g. ) tree, rock, church
class TerrainFeature : public IObject
{
private:
    vector<BoxCollider*> m_colliders;
    EffectMeshRenderer*  pEffectMeshRenderer;

public:
             TerrainFeature(
                 const TAG_RES_STATIC tag, 
                 const D3DXVECTOR3&   position,
                 const D3DXVECTOR3&   rotation,
                 const D3DXVECTOR3&   scale);

    virtual ~TerrainFeature();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void AddBoxCollider(const D3DXMATRIX&  transformationMatrix);
};