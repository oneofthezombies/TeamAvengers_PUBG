#pragma once

class UICompass : public UIObject
{
public:
    static const float WIDTH;
    static const float BEGIN_X;
    static const float RADIUS;
    static const LONG  RECT_OFFSET_X;

private:
    float              m_rotationY;
    RECT               m_rect;
    LPDIRECT3DTEXTURE9 pCompassTexture;

public:
             UICompass(UIObject* pParent);
    virtual ~UICompass();

    virtual void Render() override;

    void SetRotationY(const D3DXVECTOR3& direction);
};