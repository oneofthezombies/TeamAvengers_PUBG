#pragma once

class UIObject
{
private:
    D3DXVECTOR3 m_vPosition;
    int         m_instanceID;
    BaseObject* m_pAttachedObject;

    void DrawBorder();
    void UpdateRect();

    void UpdateViewportPosition();
    void SetParent(UIObject& parent);
    const D3DXVECTOR3& GetViewportPosition() const;

protected:
    D3DXVECTOR3 m_vViewportPosition;
    D3DXVECTOR3	m_vCenter;
    D3DXVECTOR2 m_vSize;
    RECT        m_rect;
    D3DCOLOR    m_color;

    deque<UIObject*> m_deqPChildren;
    UIObject*        m_pParent;

    void UpdateViewportPosRect();

public:
	UIObject();
	virtual ~UIObject();

    virtual void Init();
    virtual void Release();
    virtual void Update();
    virtual void Render();

    void AddChild(UIObject& child);
    void ReleaseChildren();
    void ClearChildren();
    void UpdateChildren();
    void RenderChildren();

    void SetInstanceID(const int val);
    int  GetInstanceID() const;

    void SetPosition(const D3DXVECTOR3& val);
    void SetCenter(const D3DXVECTOR3& val);

    void               SetSize(const D3DXVECTOR2& val);
    const D3DXVECTOR2& GetSize() const;

    void SetColor(const D3DCOLOR& val);

    void        AttachToObject(BaseObject& val);
    BaseObject* GetAttachedObject() const;
};
