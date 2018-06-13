#pragma once

class IObject;

class UIObject
{
private:
    int         m_instanceID;
    IObject*    m_pAttachedObject;

    void SetViewportPosition(const D3DXVECTOR3& parentViewportPos, const D3DXVECTOR3& pos);
    void SetRect(const D3DXVECTOR3& viewportPos, const D3DXVECTOR2& size, const D3DXMATRIX& transform);
    void DrawBorder();
    void RenderChildren();

protected:
    D3DXVECTOR3      m_position;
    D3DXVECTOR3      m_viewportPosition;
    D3DXVECTOR3	     m_center;
    D3DXVECTOR2      m_size;
    RECT             m_rect;
    D3DCOLOR         m_color;
    deque<UIObject*> m_children;

    void SetViewportPosRect(const D3DXVECTOR3& parentViewportPos, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXMATRIX& transform);

public:
	UIObject();
	virtual ~UIObject();

    virtual void Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform);
    virtual void Render();

    void AddChild(UIObject* pChild);
    void ClearChildren();
    void UpdateChildren(const D3DXVECTOR3& viewportPos, const D3DXMATRIX& transform);

    void SetInstanceID(const int val);
    int  GetInstanceID() const;

    void SetPosition(const D3DXVECTOR3& val);
    void SetCenter(const D3DXVECTOR3& val);

    void               SetSize(const D3DXVECTOR2& val);
    const D3DXVECTOR2& GetSize() const;

    void SetColor(const D3DCOLOR val);

    void     AttachToObject(IObject* p);
    IObject* GetAttachedObject() const;
};
