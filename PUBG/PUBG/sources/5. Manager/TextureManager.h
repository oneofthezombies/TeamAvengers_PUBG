#pragma once

#include "SingletonBase.h"

#define g_pTextureManager TextureManager::GetInstance()

class TextureManager : public SingletonBase<TextureManager>
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

    TextureManager();
    virtual ~TextureManager();

public:
	LPDIRECT3DTEXTURE9 GetTexture(string fullPath);

	void Destroy();

    friend SingletonBase<TextureManager>;
};

