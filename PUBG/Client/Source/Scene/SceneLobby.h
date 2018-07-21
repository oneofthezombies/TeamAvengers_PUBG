#pragma once
#include "IScene.h"
#include "UIButton.h"
#include "Quad.h"

class UIImage;

class ReadyButtonListener : public IUIButtonOnMouseListener
{
private:
    bool m_isReady;

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
public:
    static const D3DXVECTOR3 PLAYER_0_POSITION;
    static const D3DXVECTOR3 PLAYER_1_POSITION;
    static const D3DXVECTOR3 PLAYER_2_POSITION;
    static const D3DXVECTOR3 PLAYER_3_POSITION;

    static const D3DXQUATERNION PLAYER_0_ROTATION;
    static const D3DXQUATERNION PLAYER_1_ROTATION;
    static const D3DXQUATERNION PLAYER_2_ROTATION;
    static const D3DXQUATERNION PLAYER_3_ROTATION;

private:
    UIImage* m_pBackground;
    ReadyButtonListener m_readyButtonListener;
    Quad m_quad;

public:
    SceneLobby();
    ~SceneLobby();

    virtual void OnInit() override;
    virtual void OnUpdate() override;

    void StartPlay();
    void AddCharacters();
    void RemoveCharacters();
};

