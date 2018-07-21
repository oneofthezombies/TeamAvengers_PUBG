#include "stdafx.h"
#include "Quad.h"
#include "ComponentTransform.h"

Quad::Quad()
    : IObject(TAG_OBJECT::Idle)
    , pTexture(nullptr)
    , pEffect(nullptr)
{
    m_vertices.resize(4);
    m_vertices[0].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
    m_vertices[0].t = D3DXVECTOR2(0.0f, 1.0f);
    m_vertices[1].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    m_vertices[1].t = D3DXVECTOR2(0.0f, 0.0f);
    m_vertices[2].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    m_vertices[2].t = D3DXVECTOR2(1.0f, 0.0f);
    m_vertices[3].p = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    m_vertices[3].t = D3DXVECTOR2(1.0f, 1.0f);

    m_indices.resize(6);
    m_indices[0] = 0;
    m_indices[1] = 1;
    m_indices[2] = 2;
    m_indices[3] = 0;
    m_indices[4] = 2;
    m_indices[5] = 3;

    pEffect = Resource()()->GetEffect("./Resource/", "Quad.fx");
}

Quad::~Quad()
{
}

void Quad::OnUpdate()
{
}

void Quad::OnRender()
{
    Shader::Draw(
        pEffect, 
        nullptr, 
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World, 
            &GetTransform()->GetTransformationMatrix());

        pEffect->SetTexture("Quad_Diffuse_Tex", pTexture);
    }, 
        [this]() 
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_TRIANGLELIST, 
            0, 
            static_cast<UINT>(m_vertices.size()), 
            static_cast<UINT>(m_indices.size() / 3),
            m_indices.data(), 
            D3DFMT_INDEX16, 
            m_vertices.data(), 
            sizeof m_vertices.front());
    });
}

void Quad::SetTexture(const std::string& path, const std::string& filename)
{
    pTexture = Resource()()->GetTexture(path, filename);
}

void Quad::SetPosition(const D3DXVECTOR3& p)
{
    GetTransform()->SetPosition(p);
    GetTransform()->Update();
}

void Quad::SetRotation(const D3DXQUATERNION& r)
{
    GetTransform()->SetRotation(r);
    GetTransform()->Update();
}

void Quad::SetScale(const D3DXVECTOR3& s)
{
    GetTransform()->SetScale(s);
    GetTransform()->Update();
}
