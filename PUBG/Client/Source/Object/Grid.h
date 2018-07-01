#pragma once
#include "IObject.h"

class Grid : public IObject
{
private:
    D3DXMATRIX m_transformationMatrix;
    vector<D3DXVECTOR3> m_grays;
    vector<D3DXVECTOR3> m_whites;
    vector<D3DXVECTOR3> m_reds;
    vector<D3DXVECTOR3> m_greens;
    vector<D3DXVECTOR3> m_blues;
    LPD3DXEFFECT pEffect;

private:
    void draw(const vector<D3DXVECTOR3>& vertices, const D3DXCOLOR& color);

public:
    Grid();
    virtual ~Grid();

    void Init();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};