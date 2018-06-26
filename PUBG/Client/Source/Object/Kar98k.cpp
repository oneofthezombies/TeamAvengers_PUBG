#include "stdafx.h"
#include "Kar98k.h"
#include "SkinnedMeshController.h"

Kar98k::Kar98k()
    : IObject()
    , pSkinnedMeshController(nullptr)
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->LoadSkinnedMesh("./Resource/Kar98k/", "Kar98k.x");

    GetTransform()->SetScale(Vector3::ONE * 2.0f);

    D3DXCreateSphere(Device()(), 5.0f, 10u, 10u, &m_pSphereMesh, nullptr);
}

Kar98k::~Kar98k()
{
    SAFE_RELEASE(m_pSphereMesh);
}

void Kar98k::OnUpdate()
{
    pSkinnedMeshController->Update([this]() {});
}

void Kar98k::OnRender()
{
    pSkinnedMeshController->Render();

    auto pD = Device()();
    pD->SetRenderState(D3DRS_LIGHTING, true);

    pD->SetTexture(0, NULL);
    pD->SetMaterial(&MaterialTemplate::GetWhite());
    pD->SetTransform(
        D3DTS_WORLD, &GetTransform()->GetTransformationMatrix());
    m_pSphereMesh->DrawSubset(0);
}
