#pragma once
#include "UIObject.h"

class UIImage : public UIObject
{
private:
    LPDIRECT3DTEXTURE9 m_pTex;

public:
	UIImage(UIObject* pParent);
	virtual ~UIImage();

	virtual void Render() override;

	void SetTexture(const string& fullPath);
};

