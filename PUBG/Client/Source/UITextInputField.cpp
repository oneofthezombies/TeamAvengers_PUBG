#include "stdafx.h"
#include "UITextInputField.h"
#include "UIText.h"

enum CHILD
{
    BUTTON,
    TEXT
};

UIButton* UITextInputField::GetButton() const
{
    return static_cast<UIButton*>(m_children[CHILD::BUTTON]);
}

UIText* UITextInputField::GetText() const
{
    return static_cast<UIText*>(m_children[CHILD::TEXT]);
}

UITextInputField::UITextInputField(
    const D3DXVECTOR3& pos, 
    const string& textureDir, 
    const string& idleTexture, const string& mouseOverTexture, const string& selectTexture, 
    const TAG_FONT font)
    : UIObject()
    , m_listener()
    , m_text()
    , m_isSelected(false)
{
    SetPosition(pos);

    UIButton* pButton = new UIButton;
    pButton->SetTexture(textureDir + idleTexture, textureDir + mouseOverTexture, textureDir + selectTexture);
    SetSize(pButton->GetSize());
    AddChild(pButton);

    m_listener.SetHandle(this);
    m_listener.SetUIButton(pButton);

    UIText* pText = new UIText;
    pText->SetFont(g_pFontManager->GetFont(font));
    pText->SetSize(pButton->GetSize());
    pText->SetText(&m_text);
    pText->SetColor(D3DCOLOR_XRGB(0, 0, 0));
    pText->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
    AddChild(pText);
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
