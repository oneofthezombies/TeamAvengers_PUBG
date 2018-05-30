#pragma once
#include "UIObject.h"

class IUIButtonOnMouseListener;

class UIButton : public UIObject
{
private:
	enum State
	{
        kIdle,
        kMouseOver,
        kSelect,
	};
    static const int m_kNumState = 3;

    IUIButtonOnMouseListener*  m_pIUIButtonOnMouseListener;
    State                      m_state;
    int                        m_KeyToRespond;
    bool                       m_bPrevIsMouseOn;
    bool                       m_bCurrIsMouseOn;
    bool                       m_bIsClicked;
    vector<LPDIRECT3DTEXTURE9> m_vecTexture;

    void UpdateOnMouseEnterExit();
    void UpdateOnMouseDownUpDrag();

public:
	UIButton();
	virtual ~UIButton();

	virtual void Update() override;
	virtual void Render() override;

    void UpdateOnMouse();

	void SetTexture(const string& idle, const string& mouseOver, const string& select);
	void SetText(const LPD3DXFONT font, const LPCTSTR text);
    void SetKeyToRespond(const int key);
    void SetIUIButtonOnMouseListener(IUIButtonOnMouseListener& val);

    static UIButton* Create(const string& idlePath, const string& mouseOverPath, const string& selectPath, const D3DXVECTOR3& pos, UIObject* parent = nullptr);
};

class IUIButtonOnMouseListener
{
private:
    UIButton* m_pUIButton;
    UIObject* m_pHandle;

public:
    IUIButtonOnMouseListener();
    virtual ~IUIButtonOnMouseListener() = default;

    virtual void OnMouseEnter() = 0;
    virtual void OnMouseExit() = 0;
    virtual void OnMouseDown(const int key) = 0;
    virtual void OnMouseUp(const int key) = 0;
    virtual void OnMouseDrag(const int key) = 0;

    void      SetUIButton(UIButton& val);
    UIButton* GetUIButton() const;

    void      SetHandle(UIObject& val);
    UIObject* GetHandle() const;
};
