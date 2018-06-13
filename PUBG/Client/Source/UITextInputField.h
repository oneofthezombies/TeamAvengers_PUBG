#pragma once
#include "UIButton.h"
#include "TagFont.h"

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

    UIButton* GetButton() const;
    UIText*   GetText() const;

public:
    UITextInputField(const D3DXVECTOR3& pos, const string& textureDir, const string& idleTexture, const string& mouseOverTexture, const string& selectTexture, const TAG_FONT font);
    virtual ~UITextInputField();

    virtual void Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform) override;
    virtual void Render() override;

    void SetIsSelected(const bool val);
    bool IsSelected() const;
};

