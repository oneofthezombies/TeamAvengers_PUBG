#include "stdafx.h"
#include "UICompass.h"
#include "ScenePlay.h"

const float UICompass::WIDTH         = 1600.0f;
const float UICompass::BEGIN_X       =  400.0f;
const float UICompass::RADIUS        =  127.32398f; // radius = width / (2 * 2 * pi)
const LONG  UICompass::RECT_OFFSET_X =  250;

UICompass::UICompass(UIObject* pParent)
    : UIObject(pParent)
    , m_rotationY(0.0f)
{
    std::pair<std::string, std::string> pathFilename 
        = std::make_pair("./Resource/UI/InGame/", "compass_long.png");

    pCompassTexture = Resource()()->GetTexture(pathFilename.first, pathFilename.second);
    assert(pCompassTexture && "UICompass::UICompass(), Resource::Manager::GetTexture() failed.");

    D3DXIMAGE_INFO info;
    D3DXGetImageInfoFromFileA(
        (pathFilename.first + pathFilename.second).c_str(), 
        &info);

    m_size.x = static_cast<float>(info.Width);
    m_size.y = static_cast<float>(info.Height);

    m_rect.left = m_rect.top = 0;
    m_rect.right = static_cast<LONG>(m_size.x);
    m_rect.bottom = static_cast<LONG>(m_size.y);

    SetPosition(D3DXVECTOR3(-27.0f, 0.0f, 0.0f));
}

UICompass::~UICompass()
{
}

void UICompass::Render()
{
    Debug << "UICompass Rotation Y : " << m_rotationY << endl;

    const float fPoint = BEGIN_X + m_rotationY * RADIUS;
    const LONG  lPoint = static_cast<LONG>(fPoint);
    m_rect.left  = lPoint - RECT_OFFSET_X;
    m_rect.right = lPoint + RECT_OFFSET_X;

    Sprite()()->Draw(
        pCompassTexture, 
        &m_rect, 
        &m_center, 
        &m_viewportPosition, 
        m_color);

    UIObject::Render();
}

void UICompass::SetRotationY(const D3DXVECTOR3& direction)
{
    m_rotationY = Vector3::DirectionToRotationY(direction);
}
