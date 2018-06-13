#pragma once
#include "Singleton.h"

#define g_pTextureManager TextureManager::GetInstance()

class TextureManager : public Singleton<TextureManager>
{
private:
	unordered_map<string, LPDIRECT3DTEXTURE9> m_textures;

    TextureManager();
    virtual ~TextureManager();

public:
	LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);

	void Destroy();

    friend Singleton<TextureManager>;
};

