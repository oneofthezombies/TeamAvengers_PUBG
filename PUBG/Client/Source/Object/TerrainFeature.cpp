#include "stdafx.h"
#include "TerrainFeature.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"
#include "Light.h"

TerrainFeature::TerrainFeature(
    const TAG_RES_STATIC tag,
    const std::string&   name,
    const D3DXVECTOR3&   position,
    const D3DXVECTOR3&   rotation,
    const D3DXVECTOR3&   scale)
    : IObject(TAG_OBJECT::TerrainFeature)
    , m_name(name)
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(tag);

    Transform* tr = GetTransform();
    tr->SetPosition(position);
    tr->SetRotation(rotation);
    tr->SetScale(scale);
    tr->Update();

    // setup bounding sphere
    m_boundingSphere = pEffectMeshRenderer->GetBoundingSphere();
    m_boundingSphere.position = position;

    //Total Cell Space에 넣는것
    CurrentScene()()->InsertObjIntoTotalCellSpace(TAG_OBJECT::TerrainFeature, CurrentScene()()->GetCellIndex(position), this);
}

TerrainFeature::~TerrainFeature()
{
}

void TerrainFeature::OnUpdate()
{
    //distance culling
    D3DXVECTOR3 vLength = GetTransform()->GetPosition() - CurrentCamera()()->GetPosition();
    if (D3DXVec3Length(&vLength) < 2000.0f)
    {
        Shader()()->AddShadowSource(
            GetTransform()->GetTransformationMatrix(),
            pEffectMeshRenderer->GetEffectMesh());
    }
    
}

void TerrainFeature::OnRender()
{
    //frustum culling
    if (CurrentCamera()()->IsObjectInsideFrustum(
        m_boundingSphere.center + m_boundingSphere.position,
        m_boundingSphere.radius))
    {
        //distance culling
        D3DXVECTOR3 vlength = (m_boundingSphere.center + m_boundingSphere.position) - CurrentCamera()()->GetPosition();
        if (D3DXVec3Length(&vlength) < 10000.0f)
        {
            Device()()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
            Device()()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Device()()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

            pEffectMeshRenderer->Render(
                [this](LPD3DXEFFECT pEffect)
            {+
                pEffect->SetMatrix(
                    Shader::World,
                    &GetTransform()->GetTransformationMatrix());
            });

            Device()()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

            for (auto& bb : m_boundingBoxes)
                bb.Render();

            m_boundingSphere.Render();
        }


    }
    
}

void TerrainFeature::AddBoundingBox(const D3DXMATRIX& transformationMatrix)
{
    m_boundingBoxes.emplace_back(BoundingBox::Create(transformationMatrix));
}
