#include "stdafx.h"
#include "Bandage.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"

Bandage::Bandage(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale)
    : IObject()

    , pEffectMeshRenderer(nullptr)
    , pBoxCollider(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    //pEffectMeshRenderer->SetEffectMesh("./Resource/Bandage/", "Bandage.X");
    pEffectMeshRenderer->SetEffectMesh("./Resource/Church/", "Church.X");

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(TAG_COLLISION::Player_1_Damage);
    pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

    GetTransform()->SetPosition(pos);
    GetTransform()->SetScale(scale);
}

Bandage::~Bandage()
{
}

void Bandage::OnUpdate()
{
    pBoxCollider->Update(GetTransform()->GetTransformationMatrix());
    pEffectMeshRenderer->Render();
    pBoxCollider->Render();
}

void Bandage::OnRender()
{

}
