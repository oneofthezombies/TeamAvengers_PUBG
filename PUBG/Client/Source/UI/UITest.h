#pragma once

class Quad;

class UITest : public UIObject
{
private:
    LPDIRECT3DTEXTURE9 m_pImage;
    LPDIRECT3DTEXTURE9 m_pAlpha;
    LPDIRECT3DTEXTURE9 m_pRenderTarget;
    Quad* m_pQuad;

public:
             UITest();
    virtual ~UITest();

    virtual void Render() override;
};