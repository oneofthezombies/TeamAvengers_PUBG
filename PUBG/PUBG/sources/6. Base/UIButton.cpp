#include "stdafx.h"
#include "UIButton.h"
#include "UIManager.h"
#include "UIText.h"

UIButton::UIButton()
    : UIObject()
    , m_state(State::kIdle)
    , m_KeyToRespond(VK_LBUTTON)
    , m_pIUIButtonOnMouseListener(nullptr)
    , m_bPrevIsMouseOn(false)
    , m_bCurrIsMouseOn(false)
    , m_bIsClicked(false)
{
    m_vecTexture.resize(m_kNumState, nullptr);
}

UIButton::~UIButton()
{
}

void UIButton::Update()
{
    UpdateViewportPosRect();
    UpdateOnMouse();
    UpdateChildren();
}

void UIButton::Render()
{
	if (m_vecTexture[m_state])
	{
		RECT rect;
		SetRect(&rect, 0, 0, static_cast<int>(m_vSize.x), static_cast<int>(m_vSize.y));

        g_pSprite->Draw(m_vecTexture[m_state], &rect, &m_vCenter, &m_vViewportPosition, m_color);
	}

	UIObject::Render();
}

void UIButton::UpdateOnMouse()
{
    UpdateOnMouseEnterExit();
    UpdateOnMouseDownUpDrag();
}

void UIButton::SetTexture(const string& idle, const string& mouseOver, const string& select)
{
    const auto texMgr = g_pTextureManager;
    if (!texMgr) return;

	D3DXIMAGE_INFO info;
	m_vecTexture[State::kIdle]= texMgr->GetTexture(idle);
	m_vecTexture[State::kMouseOver] = texMgr->GetTexture(mouseOver);
	m_vecTexture[State::kSelect] = texMgr->GetTexture(select);

	D3DXGetImageInfoFromFileA(idle.c_str(), &info);
	m_vSize.x = static_cast<float>(info.Width);
	m_vSize.y = static_cast<float>(info.Height);
}

void UIButton::SetText(const LPD3DXFONT font, const LPCTSTR text)
{
	UIText* pText = new UIText;
	AddChild(*pText);
    pText->SetFont(font);
    pText->SetText(text);
    pText->SetSize(m_vSize);
}

void UIButton::SetKeyToRespond(const int key)
{
    m_KeyToRespond = key;
}

void UIButton::SetIUIButtonOnMouseListener(IUIButtonOnMouseListener& val)
{
    m_pIUIButtonOnMouseListener = &val;
}

UIButton* UIButton::Create(const string& idlePath, const string& mouseOverPath, const string& selectPath, const D3DXVECTOR3& pos, UIObject* parent)
{
    UIButton* ret = new UIButton;
    ret->SetTexture(idlePath, mouseOverPath, selectPath);
    ret->SetPosition(pos);
    
    if (parent)
        parent->AddChild(*ret);
    else
        g_pUIManager->RegisterUIObject(*ret);

    return ret;
}

void UIButton::UpdateOnMouseEnterExit()
{
    if (!m_pIUIButtonOnMouseListener) return;

    m_bPrevIsMouseOn = m_bCurrIsMouseOn;
    m_bCurrIsMouseOn = PtInRect(&m_rect, g_pKeyManager->GetCurrentMousePos());

    if (!m_bPrevIsMouseOn && m_bCurrIsMouseOn)
        m_pIUIButtonOnMouseListener->OnMouseEnter();

    if (m_bPrevIsMouseOn && !m_bCurrIsMouseOn)
        m_pIUIButtonOnMouseListener->OnMouseExit();
}

/*

TODO : 클릭 인식 뭐같음 수정해야 함

*/
void UIButton::UpdateOnMouseDownUpDrag()
{
    const auto keyMgr = g_pKeyManager;
    if (!keyMgr) return;

    switch (m_state)
    {
    case State::kIdle:
        {
            if (m_bCurrIsMouseOn)
            {
                m_state = State::kMouseOver;
            }
        }
        break;
    case State::kMouseOver:
        {
            if (m_KeyToRespond == VK_LBUTTON)
            {
                if (keyMgr->IsKeyDownMouseL())
                {
                    m_state = State::kSelect;

                    if (m_pIUIButtonOnMouseListener)
                        m_pIUIButtonOnMouseListener->OnMouseDown(m_KeyToRespond);
                }
                else if (!m_bCurrIsMouseOn)
                {
                    m_state = State::kIdle;
                }
            }
            else if (m_KeyToRespond == VK_RBUTTON)
            {
                if (keyMgr->IsKeyDownMouseR())
                {
                    m_state = State::kSelect;

                    if (m_pIUIButtonOnMouseListener)
                        m_pIUIButtonOnMouseListener->OnMouseDown(m_KeyToRespond);
                }
                else if (!m_bCurrIsMouseOn)
                {
                    m_state = State::kIdle;
                }
            }
            else
            {
                // something error
            }
        }
        break;
    case State::kSelect:
        {
            if (m_KeyToRespond == VK_LBUTTON)
            {
                if (keyMgr->IsKeyDownMouseL() && keyMgr->GetPrevIsKeyDownMouseL())
                {
                    if (m_bCurrIsMouseOn && m_bPrevIsMouseOn)
                        if (m_pIUIButtonOnMouseListener)
                            m_pIUIButtonOnMouseListener->OnMouseDrag(m_KeyToRespond);
                }
                else
                {
                    if (m_bCurrIsMouseOn)
                    {
                        m_state = State::kMouseOver;

                        if (keyMgr->GetPrevIsKeyDownMouseL())
                            if (m_pIUIButtonOnMouseListener)
                                m_pIUIButtonOnMouseListener->OnMouseUp(m_KeyToRespond);
                    }
                    else
                    {
                        m_state = State::kIdle;
                    }
                }
            }
            else if (m_KeyToRespond == VK_RBUTTON)
            {
                if (keyMgr->IsKeyDownMouseR() && keyMgr->GetPrevIsKeyDownMouseR())
                {
                    if (m_bCurrIsMouseOn && m_bPrevIsMouseOn)
                        if (m_pIUIButtonOnMouseListener)
                            m_pIUIButtonOnMouseListener->OnMouseDrag(m_KeyToRespond);
                }
                else
                {
                    if (m_bCurrIsMouseOn)
                    {
                        m_state = State::kMouseOver;

                        if (keyMgr->GetPrevIsKeyDownMouseR())
                            if (m_pIUIButtonOnMouseListener)
                                m_pIUIButtonOnMouseListener->OnMouseUp(m_KeyToRespond);
                    }
                    else
                    {
                        m_state = State::kIdle;
                    }
                }
            }
            else
            {
                // something error
            }
        }
        break;
    default:
        {
            // something error
        }
        break;
    }
}

IUIButtonOnMouseListener::IUIButtonOnMouseListener()
    : m_pUIButton(nullptr)
    , m_pHandle(nullptr)
{
}

void IUIButtonOnMouseListener::SetUIButton(UIButton& val)
{
    m_pUIButton = &val;
    m_pUIButton->SetIUIButtonOnMouseListener(*this);
}

UIButton* IUIButtonOnMouseListener::GetUIButton() const
{
    return m_pUIButton;
}

void IUIButtonOnMouseListener::SetHandle(UIObject& val)
{
    m_pHandle = &val;
}

UIObject* IUIButtonOnMouseListener::GetHandle() const
{
    return m_pHandle;
}
