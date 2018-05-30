#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(string fullPath)
{
	if (fullPath == "") return NULL;

	if (m_mapTexture.find(fullPath) == m_mapTexture.end())
	{
        D3DXCreateTextureFromFileExA(g_pDevice, fullPath.c_str(), 
                                     D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 
                                     D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
                                     D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, 
                                     &m_mapTexture[fullPath]);
	}
	return m_mapTexture[fullPath];
}

void TextureManager::Destroy()
{
	for (auto it : m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapTexture.clear();
}
