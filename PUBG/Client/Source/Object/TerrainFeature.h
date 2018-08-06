#pragma once
#include "IObject.h"
#include "TagResource.h"

class BoxCollider;
class EffectMeshRenderer;

// e.g. ) tree, rock, church
class TerrainFeature : public IObject
{
public:
    static const float DISTANCE_FOR_CULLING;

private:
    const std::string&   m_name;
    const TAG_RES_STATIC m_tagResStatic;
    EffectMeshRenderer*  pEffectMeshRenderer;

public:
             TerrainFeature(
                 const TAG_RES_STATIC tag, 
                 const std::string&   name,
                 const D3DXVECTOR3&   position,
                 const D3DXVECTOR3&   rotation,
                 const D3DXVECTOR3&   scale);

    virtual ~TerrainFeature();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void AddBoundingBox(const D3DXMATRIX& transformationMatrix);

    TAG_RES_STATIC GetTagResStatic() const;
};