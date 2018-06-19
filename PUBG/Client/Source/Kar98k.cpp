#include "stdafx.h"
#include "Kar98k.h"
#include "Renderer.h"
#include "Animator.h"

Kar98k::Kar98k()
    : IObject()
    , pRd(nullptr)
    , pAt(nullptr)
{
    pAt = AddComponent<Animator>();
    pAt->SetSkinnedMesh(
        g_pResourceManager->GetSkinnedMesh("./Resource/Kar98k/", "Kar98k.x"));

    pRd = AddComponent<SkinnedMeshRenderer>();

    pAt->SetAnimationIndex(0, false);
}

Kar98k::~Kar98k()
{
}

void Kar98k::OnUpdate()
{
    pAt->Update();
}

void Kar98k::OnRender()
{
    pRd->Render();
}
