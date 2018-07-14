#include "stdafx.h"
#include "UITest.h"
#include "UIImage.h"

UITest::UITest()
    : UIObject(nullptr)
    , pImage(nullptr)
    , pAlpha(nullptr)
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


}

UITest::~UITest()
{
}

void UITest::Render()
{
    auto d = Device()();
    d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

    d->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_MIN);

    pImage->Render();
    pAlpha->Render();

    d->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);

    UIObject::Render();
}
