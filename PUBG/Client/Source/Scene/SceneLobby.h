#pragma once
#include "IScene.h"
#include "UIButton.h"

class UIImage;

class ReadyButtonListener : public IUIButtonOnMouseListener
{
public:
    ReadyButtonListener();
    virtual ~ReadyButtonListener();

    virtual void OnMouseEnter() override;
    virtual void OnMouseExit() override;
    virtual void OnMouseDown(const int key) override;
    virtual void OnMouseUp(const int key) override;
    virtual void OnMouseDrag(const int key) override;
};

class SceneLobby : public IScene
{
private:
    UIImage* m_pBackground;
    ReadyButtonListener m_readyButtonListener;

public:
    SceneLobby();
    ~SceneLobby();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

