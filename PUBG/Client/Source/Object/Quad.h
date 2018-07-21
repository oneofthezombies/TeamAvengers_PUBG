#pragma once

class Quad : public IObject
{
private:
    std::vector<VERTEX_PT> m_vertices;
    std::vector<WORD>      m_indices;
    LPDIRECT3DTEXTURE9     pTexture;
    LPD3DXEFFECT           pEffect;

public:
             Quad();
    virtual ~Quad();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void SetTexture(const std::string& path, const std::string& filename);
    void SetPosition(const D3DXVECTOR3& p);
    void SetRotation(const D3DXQUATERNION& r);
    void SetScale(const D3DXVECTOR3& s);
};