#include "stdafx.h"
#include "Quad.h"
#include "ComponentTransform.h"

Quad::Quad()
    : IObject(TAG_OBJECT::Idle)
    , pTexture(nullptr)
    , pEffect(nullptr)
    , m_pMesh(nullptr)
{
    m_vertices.resize(4);
    m_vertices[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vertices[0].t = D3DXVECTOR2(0.0f, 1.0f);

    m_vertices[1].p = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
    m_vertices[1].t = D3DXVECTOR2(0.0f, 0.0f);

    m_vertices[2].p = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
    m_vertices[2].t = D3DXVECTOR2(1.0f, 0.0f);

    m_vertices[3].p = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
    m_vertices[3].t = D3DXVECTOR2(1.0f, 1.0f);

    m_indices.resize(6);
    m_indices[0] = 0;
    m_indices[1] = 1;
    m_indices[2] = 2;
    m_indices[3] = 0;
    m_indices[4] = 2;
    m_indices[5] = 3;

    HRESULT hr = D3DXCreateMeshFVF(static_cast<DWORD>(m_indices.size() / 3), static_cast<DWORD>(m_vertices.size()), D3DXMESH_MANAGED, VERTEX_PT::FVF, Device()(), &m_pMesh);

    assert(!FAILED(hr) && "Quad::Quad(), D3DXCreateMeshFVF() failed.");

    VERTEX_PT* pV = nullptr;
    m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
    memcpy(pV, m_vertices.data(), m_vertices.size() * sizeof VERTEX_PT);
    m_pMesh->UnlockVertexBuffer();

    WORD* pI = nullptr;
    m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
    memcpy(pI, m_indices.data(), m_indices.size() * sizeof WORD);
    m_pMesh->UnlockIndexBuffer();

    DWORD* pA = NULL;
    m_pMesh->LockAttributeBuffer(0, &pA);
    ZeroMemory(pA, m_indices.size() / 3 * sizeof DWORD);
    m_pMesh->UnlockAttributeBuffer();
}

Quad::~Quad()
{
}

void Quad::OnUpdate()
{
    D3DXVECTOR3 pos = GetTransform()->GetPosition();
    const float speed = 0.5f;

    if (Input()()->IsStayKeyDown('W'))
    {
        pos.z += speed;
    }
    if (Input()()->IsStayKeyDown('S'))
    {
        pos.z -= speed;
    }
    if (Input()()->IsStayKeyDown('A'))
    {
        pos.x -= speed;
    }
    if (Input()()->IsStayKeyDown('D'))
    {
        pos.x += speed;
    }
    if (Input()()->IsStayKeyDown('Q'))
    {
        pos.y -= speed;
    }
    if (Input()()->IsStayKeyDown('E'))
    {
        pos.y += speed;
    }

    GetTransform()->SetPosition(pos);
    GetTransform()->Update();
}

void Quad::OnRender()
{
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Quad.fx"), 
        nullptr, 
        m_pMesh,
        0, 
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World, 
            &GetTransform()->GetTransformationMatrix());

        pEffect->SetTexture("Quad_Diffuse_Tex", pTexture);
    });

    //Shader::Draw(
    //    Resource()()->GetEffect("./Resource/", "Quad.fx"),
    //    nullptr, 
    //    [this](LPD3DXEFFECT pEffect) 
    //{
    //    pEffect->SetMatrix(
    //        Shader::World, 
    //        &GetTransform()->GetTransformationMatrix());

    //    pEffect->SetTexture("Quad_Diffuse_Tex", pTexture);
    //}, 
    //    [this]() 
    //{
    //    Device()()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 
    //});
}

void Quad::SetTexture(const std::string& path, const std::string& filename)
{
    pTexture = Resource()()->GetTexture(path, filename);
    assert(pTexture && "Quad::SetTexture(), texture is null.");
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
