#include "stdafx.h"
#include "IDisplayObject.h"

IDisplayObject::IDisplayObject()
    : BaseObject()
    , m_pParent(nullptr)
    , m_pos(0.0f, 0.0f, 0.0f)
    , m_rot(0.0f, 0.0f, 0.0f)
    , m_heightOffset(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

IDisplayObject::~IDisplayObject()
{
}

void IDisplayObject::UpdateTransform()
{
    D3DXMATRIX r, t;
    D3DXMatrixRotationYawPitchRoll(&r, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixTranslation(&t, m_pos.x, m_pos.y + m_heightOffset, m_pos.z);
    m_matWorld = r * t;
}

void IDisplayObject::AddChild(IDisplayObject& val)
{
    val.m_pParent = this;
    m_vecPChild.push_back(&val);
}

D3DXVECTOR3 IDisplayObject::GetPosition() const
{
    return m_pos;
}

void IDisplayObject::SetPosition(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

D3DXVECTOR3 IDisplayObject::GetRotation() const
{
    return m_rot;
}

const D3DXMATRIX& IDisplayObject::GetWorldMatrix() const
{
    return m_matWorld;
}

const vector<IDisplayObject*>& IDisplayObject::GetChildVec() const
{
    return m_vecPChild;
}

void IDisplayObject::SetHeightOffset(const float val)
{
    m_heightOffset = val;
}

void IDisplayObject::Release()
{
    ReleaseChildren();
    BaseObject::Release();
}

void IDisplayObject::UpdateChildren()
{
    for (auto& c : m_vecPChild)
        SAFE_UPDATE(c);
}

void IDisplayObject::RenderChildren()
{
    for (auto& c : m_vecPChild)
        SAFE_RENDER(c);
}

void IDisplayObject::ReleaseChildren()
{
    for (auto c : m_vecPChild)
        SAFE_RELEASE(c);
}

void IDisplayObject::UpdatePositionYOnMap()
{
    IMap* map = g_pCurrentMap;
    if (!map) return;

    float height = 0.0f;
    map->GetHeight(height, m_pos);
    m_pos.y = height + m_heightOffset;
}
