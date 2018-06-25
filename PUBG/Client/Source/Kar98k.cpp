#include "stdafx.h"
#include "Kar98k.h"
#include "SkinnedMeshController.h"

Kar98k::Kar98k()
    : IObject()
    , pSkinnedMeshController(nullptr)
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->LoadSkinnedMesh("./Resource/Kar98k/", "Kar98k.x");

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
    pSkinnedMeshController->Update([this]() {});
}

void Kar98k::OnRender()
{
    pSkinnedMeshController->Render();
}
