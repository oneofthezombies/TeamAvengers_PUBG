#include "stdafx.h"
#include "Bandage.h"
#include "Renderer.h"
 
Bandage::Bandage()
    : IObject()
    , pEMR(nullptr)
{
    pEMR = AddComponent<EffectMeshRenderer>();
    pEMR->SetEffectMesh(
        g_pResourceManager->GetEffectMesh("./Resource/Bandage/", "Bandage.X"));
}

Bandage::~Bandage()
{
}

void Bandage::OnUpdate()
{
}

void Bandage::OnRender()
{
    pEMR->Render();
}
