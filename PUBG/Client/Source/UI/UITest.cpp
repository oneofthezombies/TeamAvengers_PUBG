#include "stdafx.h"
#include "UITest.h"
#include "UIImage.h"
#include "Quad.h"
#include "ComponentTransform.h"

UITest::UITest()
    : UIObject(nullptr)
    , m_pRenderTarget(nullptr)
    , m_pQuad(nullptr)
    , m_pImage(nullptr)
    , m_pAlpha(nullptr)
{
    m_pImage = Resource()()->GetTexture("./Resource/", "compass_long.png");
    m_pAlpha = Resource()()->GetTexture("./Resource/", "compass_long_alpha.png");

    D3DXCreateTexture(
        Device()(), 
        1600, 
        24, 
        1, 
        D3DUSAGE_RENDERTARGET, 
        D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, 
        &m_pRenderTarget);

    m_pQuad = new Quad;
    m_pQuad->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pQuad->SetScale(D3DXVECTOR3(800.0f, 12.0f, 0.0f));
}

UITest::~UITest()
{
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_DELETE(m_pQuad);
    SAFE_DELETE(m_pImage);
    SAFE_DELETE(m_pAlpha);
}

void UITest::Render()
{
    auto d = Device()();

    LPDIRECT3DSURFACE9 pHWRenderTarget = nullptr;
    d->GetRenderTarget(0, &pHWRenderTarget);

    LPDIRECT3DSURFACE9 pSurface = nullptr;
    m_pRenderTarget->GetSurfaceLevel(0, &pSurface);
    d->SetRenderTarget(0, pSurface);
    pSurface->Release();

    d->Clear(
        0, 
        nullptr, 
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 0, 0), 
        1.0f, 
        0);

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Quad.fx"),
        nullptr,
        m_pQuad->GetMesh(),
        0,
        [this](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(
            Shader::World,
            &m_pQuad->GetTransform()->GetTransformationMatrix());

        pEffect->SetTexture("Quad_Diffuse_Tex", m_pImage);
    });

    //d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

    //d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_MIN);


    //d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);

    d->SetRenderTarget(0, pHWRenderTarget);
    pHWRenderTarget->Release();

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Quad.fx"),
        nullptr,
        m_pQuad->GetMesh(),
        0,
        [this](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(
            Shader::World,
            &m_pQuad->GetTransform()->GetTransformationMatrix());

        pEffect->SetTexture("Quad_Diffuse_Tex", m_pRenderTarget);
    });
}
