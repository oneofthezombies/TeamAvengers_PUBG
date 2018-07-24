#pragma once

class UITest : public UIObject
{
private:
    UIImage* pImage;
    UIImage* pAlpha;
    LPDIRECT3DTEXTURE9 m_pRenderTarget;

public:
             UITest();
    virtual ~UITest();

    virtual void Render() override;
};