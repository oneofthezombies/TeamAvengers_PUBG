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
    Shader()()->AddShadowSource(
        GetTransform()->GetTransformationMatrix(), 
        pEffectMeshRenderer->GetEffectMesh());
}

void TerrainFeature::OnRender()
{
    pEffectMeshRenderer->Render(
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());
    });

    for (auto& bb : m_boundingBoxes)
        bb.Render();

    m_boundingSphere.Render();
}

void TerrainFeature::AddBoundingBox(const D3DXMATRIX& transformationMatrix)
{
    m_boundingBoxes.emplace_back(BoundingBox::Create(transformationMatrix));
}
