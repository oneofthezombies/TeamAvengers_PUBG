#pragma once
#include "UIObject.h"

class UIImage : public UIObject
{
private:
    LPDIRECT3DTEXTURE9 m_pTex;

public:
	UIImage();
	virtual ~UIImage();

	virtual void Render() override;

	void SetTexture(const string fullPath);

    static UIImage* Create(const string texturePath, const D3DXVECTOR3& pos, UIObject* parent = nullptr, const D3DXVECTOR2& size = D3DXVECTOR2(0.0f, 0.0f));
};

