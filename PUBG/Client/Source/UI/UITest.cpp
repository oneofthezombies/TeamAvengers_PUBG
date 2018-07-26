#include "stdafx.h"
#include "UITest.h"
#include "UIImage.h"
#include "Quad.h"
#include "ComponentTransform.h"

UITest::UITest()
    : UIObject(nullptr)
    , m_pRenderTarget(nullptr)
    , m_pDepthStencil(nullptr)
    , m_pQuad(nullptr)
    , pImage(nullptr)
    , pAlpha(nullptr)
{
    pImage = Resource()()->GetTexture("./Resource/", "compass_long.png");
    pAlpha = Resource()()->GetTexture("./Resource/", "compass_long_alpha.png");

    D3DXCreateTexture(
        Device()(), 
        1600, 
        24, 
        //1280,
        //720,
        1, 
        D3DUSAGE_RENDERTARGET, 
        D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, 
        &m_pRenderTarget);

    Device()()->CreateDepthStencilSurface(
        1600, 
        24, 
        //1280,
        //720,
        D3DFMT_D24X8, 
        D3DMULTISAMPLE_NONE, 
        0,
        true, 
        &m_pDepthStencil, 
        nullptr);

    m_pQuad = new Quad;
    m_pQuad->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pQuad->SetScale(D3DXVECTOR3(780.0f, 10.0f, 0.0f));
}

UITest::~UITest()
{
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pDepthStencil);
    SAFE_DELETE(m_pQuad);
}

void UITest::Render()
{
    auto d = Device()();

    LPDIRECT3DSURFACE9 pHWRenderTarget = nullptr;
    LPDIRECT3DSURFACE9 pHWDepthStencil = nullptr;
    d->GetRenderTarget(0, &pHWRenderTarget);
    d->GetDepthStencilSurface(&pHWDepthStencil);

    LPDIRECT3DSURFACE9 pSurface = nullptr;
    m_pRenderTarget->GetSurfaceLevel(0, &pSurface);
    d->SetRenderTarget(0, pSurface);
    pSurface->Release();

    d->SetDepthStencilSurface(m_pDepthStencil);

    d->Clear(
        0, 
        nullptr, 
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 0), 
        1.0f, 
        0);

    d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
    d->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
    d->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_SRCALPHA);
    d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

    //Sprite()()->Draw(pAlpha, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));

    d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
    d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_MIN);
    d->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    d->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

    Sprite()()->Draw(pImage, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));

    //d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    //d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    //d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    //d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
    //d->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
    //d->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_SRCALPHA);
    //d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

    ////Sprite()()->Draw(pAlpha, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));

    //d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    //d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    //d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
    //d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_MIN);
    //d->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    //d->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

    //Sprite()()->Draw(pImage, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));

    d->SetRenderTarget(0, pHWRenderTarget);
    d->SetDepthStencilSurface(pHWDepthStencil);
    pHWRenderTarget->Release();
    pHWDepthStencil->Release();

    d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
    d->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
    d->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
    d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);

    Sprite()()->Draw(m_pRenderTarget, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));
    //Sprite()()->Draw(pImage, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));
}
