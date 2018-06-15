#include "stdafx.h"
#include "Cube.h"
#include "UIText.h"

Cube::Cube()
    : IObject()
    , m_pUIText(nullptr)
    , m_isMine(false)
    , m_ID(-1)
{
    m_pUIText = new UIText(g_pFontManager->GetFont(TAG_FONT::DEFAULT), D3DXVECTOR2(100.0f, 100.0f), &m_text, D3DCOLOR_XRGB(255, 255, 255), nullptr);
}

Cube::~Cube()
{
}

void Cube::OnUpdate()
{
    if (m_isMine)
    {
        bool isMoved = false;
        if (g_pInput->IsStayKeyDown('A'))
        {
            isMoved = true;
            m_pUIText->SetPosition(m_pUIText->GetPosition() + D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
        }

        if (g_pInput->IsStayKeyDown('D'))
        {
            isMoved = true;
            m_pUIText->SetPosition(m_pUIText->GetPosition() + D3DXVECTOR3(1.0f, 0.0f, 0.0f));
        }

        if (isMoved)
            g_pCommunicator->UpdatePosition(m_pUIText->GetPosition());
    }
    else
    {
    }
}

void Cube::OnRender()
{
}

void Cube::SetIsMine(const bool val)
{
    m_isMine = val;
}

void Cube::SetID(const int val)
{
    m_ID = val;
}

int Cube::GetID() const
{
    return m_ID;
}

void Cube::SetPosition(const D3DXVECTOR3& pos)
{
    m_pUIText->SetPosition(pos);
}
