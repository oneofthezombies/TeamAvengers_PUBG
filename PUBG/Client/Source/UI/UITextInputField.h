#pragma once
#include "UIButton.h"
#include "TagClientOnly.h"

class UIText;

class UITextInputFieldListener : public IUIButtonOnMouseListener
{
public:
    UITextInputFieldListener();
    virtual ~UITextInputFieldListener();

    virtual void OnMouseEnter() override;
    virtual void OnMouseExit() override;
    virtual void OnMouseDown(const int key) override;
    virtual void OnMouseUp(const int key) override;
    virtual void OnMouseDrag(const int key) override;
};

class UITextInputField : public UIObject
{
private:
    UITextInputFieldListener m_listener;
    string m_text;
    bool m_isSelected;

    UIButton* GetUIButton() const;
    UIText*   GetUIText() const;

public:
    UITextInputField(const D3DXVECTOR3& pos, 
        const string& textureDir, const string& idleTex, const string& mouseOverTex, const string& selectTex, 
        const TAG_FONT font, const D3DCOLOR textColor, 
        UIObject* pParent);
    virtual ~UITextInputField();

    virtual void Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform) override;
    virtual void Render() override;

    void SetIsSelected(const bool val);
    bool IsSelected() const;

    void        SetText(const std::string& text);
    std::string GetText() const;
};

