#pragma once
#include "UIObject.h"

class UIImage : public UIObject
{
private:
    LPDIRECT3DTEXTURE9 m_pTex;
    bool               m_isRender;

public:
	UIImage(UIObject* pParent);
    UIImage(const string& path, const string& filename, const D3DXVECTOR3& position, IObject* pAttach, UIObject* pParent);
	virtual ~UIImage();

	virtual void Render() override;

	void               SetTexture(const string& fullPath);
    void               SetTexture(LPDIRECT3DTEXTURE9 pTexture);
    LPDIRECT3DTEXTURE9 GetTexture() const;

    void SetIsRender(const bool val);
    bool IsRender() const;
};

