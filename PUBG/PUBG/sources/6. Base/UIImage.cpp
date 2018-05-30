#include "stdafx.h"
#include "UIImage.h"
#include "UIManager.h"

UIImage::UIImage()
    : UIObject()
    , m_pTex(nullptr)
{
}

UIImage::~UIImage()
{
}

void UIImage::Render()
{
	if (m_pTex)
	{
		RECT rect;
		SetRect(&rect, 0, 0, static_cast<int>(m_vSize.x), static_cast<int>(m_vSize.y));
		g_pSprite->Draw(m_pTex, &rect, &m_vCenter, &m_vViewportPosition, m_color);
	}

	UIObject::Render();
}

void UIImage::SetTexture(const string fullPath)
{
	D3DXIMAGE_INFO info;
	m_pTex = g_pTextureManager->GetTexture(fullPath);
	D3DXGetImageInfoFromFileA(fullPath.c_str(), &info);
	m_vSize.x = static_cast<float>(info.Width);
	m_vSize.y = static_cast<float>(info.Height);
}

UIImage* UIImage::Create(const string texturePath, const D3DXVECTOR3& pos, UIObject* parent, const D3DXVECTOR2& size)
{
    UIImage* ret = new UIImage;
    ret->SetTexture(texturePath);
    ret->SetPosition(pos);

    if (!(size.x == 0.0f && size.y == 0.0f))
        ret->SetSize(size);

    if (parent)
        parent->AddChild(*ret);
    else
        g_pUIManager->RegisterUIObject(*ret);

    return ret;
}
