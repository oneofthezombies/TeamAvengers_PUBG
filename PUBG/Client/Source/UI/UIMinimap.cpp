#include "stdafx.h"
#include "UIMinimap.h"

const float UIMinimap::VISIBLE_DIMENSION = 170.0f;
const float UIMinimap::WORLD_DIMENSION   = 25600.0f;

const float UIMinimap::HALF_VISIBLE_DIMENSION 
    = UIMinimap::VISIBLE_DIMENSION * 0.5f;

void UIMinimap::set(
    const std::string& path, 
    const std::string& filename, 
    LPDIRECT3DTEXTURE9* OutPTexture, 
    D3DXVECTOR2* OutSize, 
    RECT* OutRect)
{
    assert(
        OutPTexture && 
        OutSize && 
        OutRect && 
        "UIMinimap::set(), argument is null.");

    *OutPTexture = Resource()()->GetTexture(path, filename);

    assert(
        *OutPTexture && 
        "UIMinimap::set(), Resource::Manager::GetTexture() failed.");

    const std::string fullPath(path + filename);
    D3DXIMAGE_INFO info;
    D3DXGetImageInfoFromFileA(fullPath.c_str(), &info);

    OutSize->x = static_cast<float>(info.Width);
    OutSize->y = static_cast<float>(info.Height);

    OutRect->left   = 0;
    OutRect->top    = 0;
    OutRect->right  = info.Width;
    OutRect->bottom = info.Height;
}

void UIMinimap::updateMinimapRect()
{
    D3DXVECTOR2 scale;
    scale.x = m_minimapSize.x / WORLD_DIMENSION;
    scale.y = m_minimapSize.y / WORLD_DIMENSION;

    D3DXVECTOR2 inMinimapPos;
    inMinimapPos.x = m_playerPosition.x * scale.x;
    inMinimapPos.y = m_playerPosition.y * scale.y;

    m_minimapRect.left
        = static_cast<LONG>(inMinimapPos.x - HALF_VISIBLE_DIMENSION);

    m_minimapRect.right
        = static_cast<LONG>(inMinimapPos.x + HALF_VISIBLE_DIMENSION);

    m_minimapRect.top
        = static_cast<LONG>(inMinimapPos.y - HALF_VISIBLE_DIMENSION);

    m_minimapRect.bottom
        = static_cast<LONG>(inMinimapPos.y + HALF_VISIBLE_DIMENSION);
}

UIMinimap::UIMinimap(UIObject* pParent)
    : UIObject(pParent)
    , m_playerRotationY(0.0f)
    , m_playerPosition(Vector3::ZERO)
    , m_minimapSize(Vector2::ZERO)
    , m_myStampSize(Vector2::ZERO)
    , pMinimapTexture(nullptr)
    , pMyStampTexture(nullptr)
{
    set(
        "./Resource/UI/InGame/", 
        "mini_map.png", 
        &pMinimapTexture, 
        &m_minimapSize, 
        &m_minimapRect);

    set(
        "./Resource/UI/InGame/", 
        "mini_map_me.png", 
        &pMyStampTexture, 
        &m_myStampSize, 
        &m_myStampRect);
}

UIMinimap::~UIMinimap()
{
}

void UIMinimap::Render()
{
    // TODO : refactoring, rotation offset 90 degrees

    updateMinimapRect();

    Sprite()()->Draw(
        pMinimapTexture, 
        &m_minimapRect, 
        &m_center, 
        &m_viewportPosition, 
        m_color);

    const D3DXVECTOR2 myStampViewportPos 
        = D3DXVECTOR2(m_viewportPosition.x, m_viewportPosition.y)
        + D3DXVECTOR2(HALF_VISIBLE_DIMENSION, HALF_VISIBLE_DIMENSION)
        - D3DXVECTOR2(m_myStampSize.x, m_myStampSize.y) * 0.5f;

    const float halfDimension = m_myStampSize.x * 0.5f;

    D3DXMATRIX m;
    D3DXMatrixTransformation2D(
        &m, 
        nullptr, 
        0.0f, 
        nullptr, 
        &D3DXVECTOR2(halfDimension, halfDimension),
        m_playerRotationY + D3DX_PI * 1.5f, 
        &myStampViewportPos);

    Sprite()()->SetTransform(&m);

    Sprite()()->Draw(
        pMyStampTexture,
        &m_myStampRect,
        nullptr,
        nullptr,
        m_color);

    UIObject::Render();
}

void UIMinimap::SetPlayerRotationY(const D3DXVECTOR3& direction)
{
    m_playerRotationY = Vector3::DirectionToRotationY(direction);
}

void UIMinimap::SetPlayerPosition(const D3DXVECTOR3& position)
{
    m_playerPosition.x = position.x;
    m_playerPosition.y = WORLD_DIMENSION - position.z;
}
