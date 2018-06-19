#pragma once

struct VERTEX_PC
{
    D3DXVECTOR3 p;
    D3DCOLOR    c;

    VERTEX_PC();
    VERTEX_PC(const D3DXVECTOR3& p, const D3DCOLOR c);

    enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct VERTEX_RHWC
{
    D3DXVECTOR4 p;
    D3DCOLOR    c;

    VERTEX_RHWC();
    VERTEX_RHWC(const D3DXVECTOR4& p, const D3DCOLOR c);
    VERTEX_RHWC(const float x, const float y, const float z, const float w, const D3DCOLOR c);

    enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
};