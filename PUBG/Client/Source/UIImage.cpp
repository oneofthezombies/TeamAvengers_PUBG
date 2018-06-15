#include "stdafx.h"
#include "UIImage.h"
#include "UIManager.h"

UIImage::UIImage(UIObject* pParent)
    : UIObject(pParent)
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
		::SetRect(&rect, 0, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y));
		g_pSprite->Draw(m_pTex, &rect, &m_center, &m_viewportPosition, m_color);
	}

	UIObject::Render();
}

void UIImage::SetTexture(const string& fullPath)
{
	D3DXIMAGE_INFO info;
	m_pTex = g_pTextureManager->GetTexture(fullPath);
	D3DXGetImageInfoFromFileA(fullPath.c_str(), &info);
	m_size.x = static_cast<float>(info.Width);
	m_size.y = static_cast<float>(info.Height);
}
