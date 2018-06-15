#include "stdafx.h"
#include "TextureManager.h"

TextureManager::TextureManager()
    : Singleton<TextureManager>()
{
}

TextureManager::~TextureManager()
{
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(const string& fullPath)
{
    const auto search = m_textures.find(fullPath);
	if (search == m_textures.end())
	{
        const auto hr = D3DXCreateTextureFromFileExA(g_pDevice, fullPath.c_str(), 
            D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, 
            D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
            NULL, NULL, &m_textures[fullPath]);

        assert(!FAILED(hr) && "TextureManager::GetTexture() failed.");
	}
	return m_textures[fullPath];
}

void TextureManager::Destroy()
{
	for (auto& t : m_textures)
		SAFE_RELEASE(t.second);

    m_textures.clear();
}
