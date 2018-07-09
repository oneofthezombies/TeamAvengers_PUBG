#include "stdafx.h"
#include "Head.h"
#include "SkinnedMeshController.h"
#include "ResourceInfo.h"
#include "DirectionalLight.h"

Head::Head()
    : IObject(TAG_OBJECT::Idle)
    , pSkinnedMeshController(nullptr)
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    const auto pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim);
    pSkinnedMeshController->SetSkinnedMesh(Resource()()->GetSkinnedMesh(pathFilename.first, pathFilename.second));
}

Head::~Head()
{
}

void Head::OnUpdate()
{
    D3DXMATRIX m;
    D3DXMatrixTransformation(&m, nullptr, nullptr, &(Vector3::ONE * 10.0f), nullptr, nullptr, &D3DXVECTOR3(0.0f, 100.0f, 0.0f));
    pSkinnedMeshController->UpdateAnimation();
    pSkinnedMeshController->UpdateModel();
    pSkinnedMeshController->Render(m, [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });
}

void Head::OnRender()
{
}
