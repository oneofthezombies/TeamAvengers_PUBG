#include "stdafx.h"
#include "UIButton.h"
#include "UIManager.h"
#include "UIText.h"

UIButton::UIButton(const D3DXVECTOR3& pos, 
    const string& textureDir, const string& idleTex, const string& mouseOverTex, const string& selectTex, 
    IUIButtonOnMouseListener* pListener, 
    UIObject* pParent, 
    const LPD3DXFONT font, const string& text, const D3DCOLOR textColor)
    : UIObject(pParent)
    , m_state(STATE::IDLE)
    , m_keyToRespond(VK_LBUTTON)
    , m_pListener(nullptr)
    , m_prevIsMouseOn(false)
    , m_isMouseOn(false)
    , m_isClicked(false)
{
    for (auto& t : m_textures)
        t = nullptr;

    SetPosition(pos);
    SetTexture(textureDir + idleTex, textureDir + mouseOverTex, textureDir + selectTex);

    if (pListener)
        pListener->SetUIButton(this);

    if (font)
        SetText(font, text, textColor);
}

UIButton::~UIButton()
{
}

void UIButton::Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform)
{
    SetViewportPosRect(parentViewportPos, m_position, m_size, transform);
    UpdateOnMouse();
    UpdateChildren(m_viewportPosition, transform);
}

void UIButton::Render()
{
	if (m_textures[m_state])
	{
        RECT rect { 0, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y) };
        g_pSprite->Draw(m_textures[m_state], &rect, &m_center, &m_viewportPosition, m_color);
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
    const auto texMgr = g_pResourceManager;
    if (!texMgr) return;

	D3DXIMAGE_INFO info;
	m_textures[STATE::IDLE]       = texMgr->GetTexture(idle);
	m_textures[STATE::MOUSE_OVER] = texMgr->GetTexture(mouseOver);
	m_textures[STATE::SELECT]     = texMgr->GetTexture(select);

	D3DXGetImageInfoFromFileA(idle.c_str(), &info);
	m_size.x = static_cast<float>(info.Width);
	m_size.y = static_cast<float>(info.Height);
}

void UIButton::SetText(const LPD3DXFONT font, const string& text, const D3DCOLOR textColor)
{
    UIText* pText = new UIText(font, m_size, text, textColor, this);
}

void UIButton::SetKeyToRespond(const int key)
{
    m_keyToRespond = key;
}

void UIButton::SetListener(IUIButtonOnMouseListener* p)
{
    if (!p) return;

    m_pListener = p;
}

void UIButton::UpdateOnMouseEnterExit()
{
    if (!m_pListener) return;

    const auto input = g_pInput;
    if (!input) return;

    m_prevIsMouseOn = m_isMouseOn;
    m_isMouseOn = PtInRect(&m_rect, input->GetCurrentMousePos());

    if (!m_prevIsMouseOn && m_isMouseOn)
        m_pListener->OnMouseEnter();

    if (m_prevIsMouseOn && !m_isMouseOn)
        m_pListener->OnMouseExit();
}

/*

TODO : add m_prevState

*/
void UIButton::UpdateOnMouseDownUpDrag()
{
    const auto input = g_pInput;
    if (!input) return;

    switch (m_state)
    {
    case STATE::IDLE:
        {
            if (m_isMouseOn)
                m_state = STATE::MOUSE_OVER;
        }
        break;
    case STATE::MOUSE_OVER:
        {
            if (m_keyToRespond == VK_LBUTTON)
            {
                if (input->IsKeyDownMouseL())
                {
                    m_state = STATE::SELECT;

                    if (m_pListener) m_pListener->OnMouseDown(m_keyToRespond);
                }
                else if (!m_isMouseOn)
                {
                    m_state = STATE::IDLE;
                }
            }
            else if (m_keyToRespond == VK_RBUTTON)
            {
                if (input->IsKeyDownMouseR())
                {
                    m_state = STATE::SELECT;

                    if (m_pListener) m_pListener->OnMouseDown(m_keyToRespond);
                }
                else if (!m_isMouseOn)
                {
                    m_state = STATE::IDLE;
                }
            }
            else
            {
                // something error
            }
        }
        break;
    case STATE::SELECT:
        {
            if (m_keyToRespond == VK_LBUTTON)
            {
                if (input->IsKeyDownMouseL() && 
                    input->GetPrevIsKeyDownMouseL())
                {
                    if (m_isMouseOn && 
                        m_prevIsMouseOn)
                        if (m_pListener) m_pListener->OnMouseDrag(m_keyToRespond);
                }
                else
                {
                    if (m_isMouseOn)
                    {
                        m_state = STATE::MOUSE_OVER;

                        if (input->GetPrevIsKeyDownMouseL())
                            if (m_pListener) m_pListener->OnMouseUp(m_keyToRespond);
                    }
                    else
                    {
                        m_state = STATE::IDLE;
                    }
                }
            }
            else if (m_keyToRespond == VK_RBUTTON)
            {
                if (input->IsKeyDownMouseR() && 
                    input->GetPrevIsKeyDownMouseR())
                {
                    if (m_isMouseOn && 
                        m_prevIsMouseOn)
                        if (m_pListener) m_pListener->OnMouseDrag(m_keyToRespond);
                }
                else
                {
                    if (m_isMouseOn)
                    {
                        m_state = STATE::MOUSE_OVER;

                        if (input->GetPrevIsKeyDownMouseR())
                            if (m_pListener) m_pListener->OnMouseUp(m_keyToRespond);
                    }
                    else
                    {
                        m_state = STATE::IDLE;
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

void IUIButtonOnMouseListener::SetUIButton(UIButton* p)
{
    if (!p) return;

    m_pUIButton = p;
    m_pUIButton->SetListener(this);
}

UIButton* IUIButtonOnMouseListener::GetUIButton() const
{
    return m_pUIButton;
}

void IUIButtonOnMouseListener::SetHandle(UIObject* p)
{
    if (!p) return;

    m_pHandle = p;
}
