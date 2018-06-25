#include "stdafx.h"
#include "Bandage.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"

Bandage::Bandage()
    : IObject()

    , pEffectMeshRenderer(nullptr)
    , pBoxCollider(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    //pEffectMeshRenderer->SetEffectMesh("./Resource/Bandage/", "Bandage.X");
    pEffectMeshRenderer->SetEffectMesh("./Resource/Church/", "Church.X");

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(TAG_COLLISION::DAMAGE_OF_PLAYER_2);
    pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

    GetTransform()->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
    GetTransform()->SetPosition(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));
}

Bandage::~Bandage()
{
}

void Bandage::OnUpdate()
{
    pBoxCollider->Update(GetTransform()->GetTransformationMatrix());
}

void Bandage::OnRender()
{
    pEffectMeshRenderer->Render();
    pBoxCollider->Render();
}
