#pragma once

#include "UIObject.h"

class UIMinimap : public UIObject
{
public:
    static const float VISIBLE_DIMENSION;
    static const float HALF_VISIBLE_DIMENSION;
    static const float WORLD_DIMENSION;

private:
    float              m_playerRotationY;
    D3DXVECTOR2        m_playerPosition;

    LPDIRECT3DTEXTURE9 pMinimapTexture;
    RECT               m_minimapRect;
    D3DXVECTOR2        m_minimapSize;

    LPDIRECT3DTEXTURE9 pMyStampTexture;
    RECT               m_myStampRect;
    D3DXVECTOR2        m_myStampSize;

private:
    void set(
        const std::string& path, 
        const std::string& filename, 
        LPDIRECT3DTEXTURE9* OutPTexture, 
        D3DXVECTOR2* OutSize,
        RECT* OutRect);
    void updateMinimapRect();

public:
             UIMinimap(UIObject* pParent);
    virtual ~UIMinimap();

    virtual void Render() override;

    void SetPlayerRotationY(const D3DXVECTOR3& direction);
    void SetPlayerPosition(const D3DXVECTOR3& position);
};