#include "stdafx.h"
#include "UITest.h"
#include "UIImage.h"

UITest::UITest()
    : UIObject(nullptr)
    , pImage(nullptr)
    , pAlpha(nullptr)
    , m_pRenderTarget(nullptr)
{
    pImage = new UIImage(
        "./Resource/UI/InGame/", 
        "compass.png", 
        Vector3::ZERO, 
        nullptr, 
        nullptr);

    pAlpha = new UIImage(
        "./Resource/UI/InGame/", 
        "compass_bg_transparent.png", 
        D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
        nullptr, 
        nullptr);

    D3DXCreateTexture(
        Device()(), 
        1600, 
        24, 
        1, 
        D3DUSAGE_RENDERTARGET, 
        D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, 
        &m_pRenderTarget);
}

UITest::~UITest()
{
    SAFE_RELEASE(m_pRenderTarget);
}

void UITest::Render()
{
    LPDIRECT3DSURFACE9 pHWRenderTarget = nullptr;
    Device()()->GetRenderTarget(0, &pHWRenderTarget);

    auto d = Device()();
    d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

    d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_MIN);

    pImage->Render();
    pAlpha->Render();

    d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);

    UIObject::Render();
}
