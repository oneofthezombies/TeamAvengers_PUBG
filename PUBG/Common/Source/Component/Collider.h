#pragma once
#include "ComponentTransform.h"
#include "TagCollision.h"

class IObject;
class ICollisionListener;

class Collider : public Component
{
public:
    enum Type
    {
        kBox,

        // Not yet used
        kSphere,
    };

private:
    ICollisionListener* m_pListener;
    Type                m_type;
    TAG_COLLISION       m_tag;

protected:
    D3DXVECTOR3 m_vCenter;

    Collider(IObject* pOwner, const Type type);

public:
    virtual ~Collider();

    Type        GetType() const;

    void        SetCenter(const D3DXVECTOR3& center);
    D3DXVECTOR3 GetCenter() const;
    
    void                SetListener(ICollisionListener* pListener);
    ICollisionListener* GetListener() const;

    void          SetTag(const TAG_COLLISION tag);
    TAG_COLLISION GetTag() const;
};

class SphereCollider : public Collider
{
private:
    float m_radius;

public:
    SphereCollider(IObject* pOwner);
    virtual ~SphereCollider() = default;

    void Init(const float radius);
    void Update(const D3DXMATRIX& transform);
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

public:
    BoxCollider(IObject* pOwner);
    virtual ~BoxCollider();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void Update(const D3DXMATRIX& transform);

    void              SetExtent(const D3DXVECTOR3& extent);
    D3DXVECTOR3       GetExtent() const;
    const D3DXMATRIX& GetTransform() const;
};

class ICollisionListener : public Component
{
protected:
    ICollisionListener(IObject* pOwner);

public:
    virtual ~ICollisionListener();

    virtual void OnCollisionEnter(const Collider& other) = 0;
    virtual void OnCollisionExit(const Collider& other) = 0;
    virtual void OnCollisionStay(const Collider& other) = 0;
};
