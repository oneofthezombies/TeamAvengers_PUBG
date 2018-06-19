#include "stdafx.h"
#include "UITextInputField.h"
#include "UIText.h"

enum CHILD
{
    UIBUTTON,
    UITEXT
};

UIButton* UITextInputField::GetUIButton() const
{
    return static_cast<UIButton*>(m_children[CHILD::UIBUTTON]);
}

UIText* UITextInputField::GetUIText() const
{
    return static_cast<UIText*>(m_children[CHILD::UITEXT]);
}

UITextInputField::UITextInputField(
    const D3DXVECTOR3& pos, 
    const string& textureDir, const string& idleTex, const string& mouseOverTex, const string& selectTex, 
    const TAG_FONT font, const D3DCOLOR textColor,
    UIObject* pParent)
    : UIObject(pParent)
    , m_listener()
    , m_text()
    , m_isSelected(false)
{
    SetPosition(pos);

    UIButton* pButton = new UIButton(D3DXVECTOR3(0.0f, 0.0f, 0.0f), textureDir, idleTex, mouseOverTex, selectTex, &m_listener, this, nullptr, "", D3DCOLOR_XRGB(0, 0, 0));

    SetSize(pButton->GetSize());

    m_listener.SetHandle(this);

    UIText* pText = new UIText(g_pFontManager->GetFont(font), GetSize(), &m_text, textColor, this);
    pText->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
}

UITextInputField::~UITextInputField()
{
}

void UITextInputField::Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform)
{
    SetViewportPosRect(parentViewportPos, m_position, m_size, transform);

    if (m_isSelected)
    {
        const auto c = g_pInput->GetChar();
        if (c == '\b')
        {
            if (!m_text.empty())
                m_text.pop_back();
        }
        else if (c != -1)
        {
            m_text += c;
        }

        Debug << "Text : " << m_text << endl;
    }

    UpdateChildren(m_viewportPosition, transform);
}

void UITextInputField::Render()
{
    UIObject::Render();
}

void UITextInputField::SetIsSelected(const bool val)
{
    m_isSelected = val;

    cout << std::boolalpha << m_isSelected << endl;
}

bool UITextInputField::IsSelected() const
{
    return false;
}

string UITextInputField::GetText() const
{
    return m_text;
}

UITextInputFieldListener::UITextInputFieldListener()
    : IUIButtonOnMouseListener()
{
}

UITextInputFieldListener::~UITextInputFieldListener()
{
}

void UITextInputFieldListener::OnMouseEnter()
{
}

void UITextInputFieldListener::OnMouseExit()
{
}

void UITextInputFieldListener::OnMouseDown(const int key)
{
}

void UITextInputFieldListener::OnMouseUp(const int key)
{
    auto pHandle = GetHandle<UITextInputField>();
    pHandle->SetIsSelected(true);
}

void UITextInputFieldListener::OnMouseDrag(const int key)
{
}
