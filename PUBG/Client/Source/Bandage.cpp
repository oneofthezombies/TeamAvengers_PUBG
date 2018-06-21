#include "stdafx.h"
#include "Bandage.h"
#include "Renderer.h"
#include "Collider.h"

Bandage::Bandage()
    : IObject()
    , pEffectMeshRenderer(nullptr)
    , pBoxCollider(nullptr)
    , pBoxColliderRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh("./Resource/Bandage/", "Bandage.X");

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(TAG_COLLISION::DAMAGE_OF_PLAYER_2);
    pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

    pBoxColliderRenderer = AddComponent<BoxColliderRenderer>();
}

Bandage::~Bandage()
{
}

void Bandage::OnUpdate()
{
}

void Bandage::OnRender()
{
    pEffectMeshRenderer->Render();
    pBoxColliderRenderer->Render();
}
