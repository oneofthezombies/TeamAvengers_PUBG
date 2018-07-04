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
    : IObject()
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(tag);

    Transform* tr = GetTransform();
    tr->SetPosition(position);
    tr->SetRotation(rotation);
    tr->SetScale(scale);
    tr->Update();
}

TerrainFeature::~TerrainFeature()
{
    for (auto c : m_colliders)
        SAFE_DELETE(c);
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

    for (auto c : m_colliders)
        c->Render();
}

void TerrainFeature::AddBoxCollider(const D3DXMATRIX& transformationMatrix)
{
    BoxCollider* pBoxCollider = new BoxCollider(this);
    pBoxCollider->Init(transformationMatrix);
    pBoxCollider->SetTagCollision(TAG_COLLISION::Impassable);

    m_colliders.emplace_back(pBoxCollider);
}
