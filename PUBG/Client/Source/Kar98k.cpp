#include "stdafx.h"
#include "Kar98k.h"
#include "Renderer.h"
#include "MeshFilterAndAnimator.h"

Kar98k::Kar98k()
    : IObject()
    , pMeshFilter(nullptr)
    , pAnimator(nullptr)
    , pSkinnedMeshRenderer(nullptr)
{
    pMeshFilter = AddComponent<MeshFilter>();
    pMeshFilter->SetSkinnedMesh("./Resource/Kar98k/", "Kar98k.x");

    pAnimator = AddComponent<Animator>();
    pAnimator->SetAnimationIndex(0, false);

    pSkinnedMeshRenderer = AddComponent<SkinnedMeshRenderer>();

    //auto tr = GetTransform();
    //D3DXMATRIX m;
    //D3DXMatrixScaling(&m, 0.1f, 0.1f, 0.1f);
    //tr->SetTransformationMatrix(m);
}

Kar98k::~Kar98k()
{
}

void Kar98k::OnUpdate()
{
    pAnimator->Update();
    pMeshFilter->Update();
}

void Kar98k::OnRender()
{
    pSkinnedMeshRenderer->Render();
}
