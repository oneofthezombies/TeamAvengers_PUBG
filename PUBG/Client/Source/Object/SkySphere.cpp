#include "stdafx.h"
#include "SkySphere.h"
#include "EffectMeshRenderer.h"

SkySphere::SkySphere()
    : IObject(TAG_OBJECT::Idle)
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(TAG_RES_STATIC::SkySphere);

    //cout << "SkySphere radius : " << pEffectMeshRenderer->GetBoundingSphere().radius * 20.0f << endl;

    const float scale = 20.0f;
    GetTransform()->SetScale(Vector3::ONE * 20.0f);
    const float mapHalfWidth = 12800.0f;
    GetTransform()->SetPosition(D3DXVECTOR3(mapHalfWidth, 0.0f, mapHalfWidth));
    GetTransform()->Update();
}

SkySphere::~SkySphere()
{
}

void SkySphere::OnUpdate()
{
    //D3DXVECTOR3 pos = GetTransform()->GetPosition();

    //const float factor = 100.0f;

    //if (Input()()->IsStayKeyDown(VK_UP))    { pos.y += factor; };
    //if (Input()()->IsStayKeyDown(VK_DOWN))  { pos.y -= factor; };

    //Debug << "skysphere pos : " << pos << endl;

    //GetTransform()->SetPosition(pos);
    //GetTransform()->Update();

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
