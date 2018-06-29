#include "stdafx.h"
#include "SkySphere.h"
#include "EffectMeshRenderer.h"

SkySphere::SkySphere()
    : IObject()
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(TAG_RES_STATIC::SkySphere);
}

SkySphere::~SkySphere()
{
}

void SkySphere::OnUpdate()
{
    Transform* pTr = GetTransform();
    D3DXVECTOR3 scale = pTr->GetScale();

    if (GetAsyncKeyState('9') & 0x8000)
    {
        scale -= Vector3::ONE * 0.01f;
    }
    if (GetAsyncKeyState('0') & 0x8000)
    {
        scale += Vector3::ONE * 0.01f;
    }

    pTr->SetScale(scale);
    pTr->Update();

    Debug << "sky sphere scale : " << scale << '\n';
}

void SkySphere::OnRender()
{
    pEffectMeshRenderer->Render(
        [this](LPD3DXEFFECT pEffect)
    {
        const GlobalVariableKey* pKey = Shader()()->GetGlobalVariableKey();
        pEffect->SetMatrix(
            pKey->m_pWorld,
            &GetTransform()->GetTransformationMatrix());
    });
}
