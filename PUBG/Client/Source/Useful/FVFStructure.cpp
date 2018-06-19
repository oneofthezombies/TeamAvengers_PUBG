#include "stdafx.h"
#include "FVFStructure.h"

VERTEX_PC::VERTEX_PC()
    : p(0.0f, 0.0f, 0.0f)
    , c(D3DCOLOR_XRGB(0, 0, 0))
{
}

VERTEX_PC::VERTEX_PC(const D3DXVECTOR3& p, const D3DCOLOR c)
    : p(p)
    , c(c)
{
}

VERTEX_RHWC::VERTEX_RHWC()
    : p(0.0f, 0.0f, 0.0f, 0.0f)
    , c(D3DCOLOR_XRGB(0, 0, 0))
{
}

VERTEX_RHWC::VERTEX_RHWC(const D3DXVECTOR4& p, const D3DCOLOR c)
    : p(p)
    , c(c)
{
}

VERTEX_RHWC::VERTEX_RHWC(const float x, const float y, const float z, const float w, const D3DCOLOR c)
    : p(x, y, z, w)
    , c(c)
{
}