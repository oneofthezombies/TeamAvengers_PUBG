#pragma once
#include "Component.h"
#include "CollisionManager.h"

class Object;
class ICollisionListener;

class Collider : public Component
{
public:
    enum Type
    {
        kBox,

        // Not yet used
        kSphere
    };

private:
    ICollisionListener* m_pListener;
    Type                m_type;
    CollisionTag        m_tag;

protected:
    D3DXVECTOR3 m_vCenter;
    D3DCOLOR    m_color;

    Collider(Object* owner, const Type type);

public:
    virtual ~Collider();

    virtual void Render();
    Type GetType() const;
    void SetColor(const D3DCOLOR color);
    D3DXVECTOR3 GetCenter() const;
    
    void                SetListener(ICollisionListener& Listener);
    ICollisionListener* GetListener() const;

    void         SetTag(const CollisionTag tag);
    CollisionTag GetTag() const;
};

class SphereCollider : public Collider
{
private:
    float m_radius;

public:
    SphereCollider(Object* owner);
    virtual ~SphereCollider() = default;

    void Init(const float radius);
    void Update(const D3DXMATRIX& transform);
    virtual void Render() override;
};

class BoxCollider : public Collider
{
private:
    D3DXVECTOR3 m_vExtent;

    // row 0 : x, y and z of axis 0
    // row 1 : x, y and z of axis 1
    // row 2 : x, y and z of axis 2
    // row 3 : x, y and z of translation
    D3DXMATRIX m_mTransform;
    vector<VERTEX_PC> m_vertices;

public:
    BoxCollider(Object* owner);
    virtual ~BoxCollider();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void Update(const D3DXMATRIX& transform);
    virtual void Render() override;

    D3DXVECTOR3 GetExtent() const;
    const D3DXMATRIX& GetTransform() const;
};

class ICollisionListener : public Component
{
protected:
    ICollisionListener(Object* pOwner);

public:
    virtual ~ICollisionListener();

    virtual void OnCollisionEnter(const Collider& other) = 0;
    virtual void OnCollisionExit(const Collider& other) = 0;
    virtual void OnCollisionStay(const Collider& other) = 0;
};
