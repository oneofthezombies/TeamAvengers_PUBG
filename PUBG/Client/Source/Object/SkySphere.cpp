#include "stdafx.h"
#include "SkySphere.h"
#include "EffectMeshRenderer.h"

SkySphere::SkySphere()
    : IObject(TAG_OBJECT::Idle)
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(TAG_RES_STATIC::SkySphere);

    const float scale = 20.0f;
    GetTransform()->SetScale(Vector3::ONE * 20.0f);
    GetTransform()->Update();
}

SkySphere::~SkySphere()
{
}

void SkySphere::OnUpdate()
{
    //Transform* pTr = GetTransform();
    //D3DXVECTOR3 scale = pTr->GetScale();

    //if (GetAsyncKeyState('9') & 0x8000)
    //{
    //    scale -= Vector3::ONE * 0.01f;
    //}
    //if (GetAsyncKeyState('0') & 0x8000)
    //{
    //    scale += Vector3::ONE * 0.01f;
    //}

    //pTr->SetScale(scale);
    //pTr->Update();

    //Debug << "sky sphere scale : " << scale << '\n';
}

void SkySphere::OnRender()
{
    pEffectMeshRenderer->Render(
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());
    });
}
