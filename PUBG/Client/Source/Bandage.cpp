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
    //pEffectMeshRenderer->SetEffectMesh("./Resource/Bandage/", "Bandage.X");
    pEffectMeshRenderer->SetEffectMesh("./Resource/Church/", "Church.X");

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(TAG_COLLISION::DAMAGE_OF_PLAYER_2);
    pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

    pBoxColliderRenderer = AddComponent<BoxColliderRenderer>();

    auto tr = GetTransform();
    D3DXMATRIX m;
    D3DXMatrixScaling(&m, 0.1f, 0.1f, 0.1f);
    tr->SetTM(m);
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
