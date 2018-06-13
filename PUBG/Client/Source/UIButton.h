#pragma once
#include "UIObject.h"

class IUIButtonOnMouseListener;

class UIButton : public UIObject
{
private:
    enum STATE
    {
        IDLE,
        MOUSE_OVER,
        SELECT
    };
    static const int NUM_STATE = 3;

    using pListener_t = IUIButtonOnMouseListener*;
    using pTexture_t  = LPDIRECT3DTEXTURE9;
    using textures_t  = array<pTexture_t, 3>;

    pListener_t m_pListener;
    STATE       m_state;
    int         m_keyToRespond;
    bool        m_prevIsMouseOn;
    bool        m_isMouseOn;
    bool        m_isClicked;
    textures_t  m_textures;

    void UpdateOnMouseEnterExit();
    void UpdateOnMouseDownUpDrag();

public:
	UIButton();
	virtual ~UIButton();

	virtual void Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform) override;
	virtual void Render() override;

    void UpdateOnMouse();

	void SetTexture(const string& idle, const string& mouseOver, const string& select);
	void SetText(const LPD3DXFONT font, const string& text);
    void SetKeyToRespond(const int key);
    void SetListener(IUIButtonOnMouseListener* p);
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

    void      SetUIButton(UIButton* p);
    UIButton* GetUIButton() const;

    void      SetHandle(UIObject* p);

    template<typename T = UIObject, enable_if_t<is_base_of_v<UIObject, T>, int> = 0>
    T* GetHandle() const
    {
        return static_cast<T*>(m_pHandle);
    }
};
