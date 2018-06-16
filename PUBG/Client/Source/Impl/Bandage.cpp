#include "stdafx.h"
#include "Bandage.h"
#include "Loader.h"

Bandage::Bandage()
    : IObject()
    , m_pRenderer(nullptr)
{
    m_pRenderer = AddComponent<Renderer>();
    m_pRenderer->SetEffectMesh(
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
    m_pRenderer->Render();
}
