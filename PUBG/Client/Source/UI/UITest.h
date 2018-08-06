#pragma once

class Quad;

class UITest : public UIObject
{
private:
    LPDIRECT3DTEXTURE9 m_pRenderTarget;
    LPDIRECT3DSURFACE9 m_pDepthStencil;
    Quad*              m_pQuad;
    LPDIRECT3DTEXTURE9 pImage;
    LPDIRECT3DTEXTURE9 pAlpha;

public:
             UITest();
    virtual ~UITest();

    virtual void Render() override;
};