#include "stdafx.h"
#include "TerrainFeature.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"
#include "DirectionalLight.h"

TerrainFeature::TerrainFeature(
    const TAG_RES_STATIC tag,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const D3DXVECTOR3& scale)
    : IObject(TAG_OBJECT::TerrainFeature)
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

    CurrentScene()()->InsertObjIntoTotalCellSpace(TAG_OBJECT::TerrainFeature, CurrentScene()()->GetCellIndex(position), this);
}

TerrainFeature::~TerrainFeature()
{
}

void TerrainFeature::OnUpdate()
{
    // do nothing
}

void TerrainFeature::OnRender()
{
    pEffectMeshRenderer->Render(
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });

    for (auto& bb : m_boundingBoxes)
        bb.Render();

    m_boundingSphere.Render();
}

void TerrainFeature::AddBoundingBox(const D3DXMATRIX& transformationMatrix)
{
    m_boundingBoxes.emplace_back(BoundingBox::Create(transformationMatrix));
}
