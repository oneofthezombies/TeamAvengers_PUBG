#pragma once
#include "IScene.h"
#include "UIButton.h"

class UIObject;

class LoginButtonListener : public IUIButtonOnMouseListener
{
public:
    LoginButtonListener();
    virtual ~LoginButtonListener();

    virtual void OnMouseEnter() override;
    virtual void OnMouseExit() override;
    virtual void OnMouseDown(const int key) override;
    virtual void OnMouseUp(const int key) override;
    virtual void OnMouseDrag(const int key) override;
};

class SceneLogin : public IScene
{
private:
    LoginButtonListener m_buttonListener;
    vector<UIObject*>   m_UIObjects;

public:
    SceneLogin();
    virtual ~SceneLogin();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
    virtual void OnRender() override;
};
