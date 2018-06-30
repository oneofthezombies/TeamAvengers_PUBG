#pragma once
#include "ComponentTransform.h"
#include "TagClientOnly.h"

using onCollisionCallback_t =
    function<void(Collider* pPerpetrator, Collider* pVictim)>;

class IObject;

class Collider : public Component
{
public:
    enum class TYPE
    {
        BOX,
        SPHERE,
    };

private:
    TYPE                m_typeCollider;
    TAG_COLLISION       m_tagCollision;

    deque<onCollisionCallback_t> onCollisionEnterCallbacks;
    deque<onCollisionCallback_t> onCollisionStayCallbacks;
    deque<onCollisionCallback_t> onCollisionExitCallbacks;

protected:
    D3DXVECTOR3 m_center;
    D3DCOLOR    m_color;

    Collider(IObject* pOwner, const TYPE type);

public:
    virtual ~Collider();

    TYPE        GetType() const;

    const D3DXVECTOR3& GetCenter() const;

    void          SetTag(const TAG_COLLISION tag);
    TAG_COLLISION GetTag() const;

    void AddOnCollisionEnterCallback(const onCollisionCallback_t& callback);
    void AddOnCollisionStayCallback(const onCollisionCallback_t& callback);
    void AddOnCollisionExitCallback(const onCollisionCallback_t& callback);

    const deque<onCollisionCallback_t>& GetOnCollisionEnterCallbacks() const;
    const deque<onCollisionCallback_t>& GetOnCollisionStayCallbacks() const;
    const deque<onCollisionCallback_t>& GetOnCollisionExitCallbacks() const;
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
    D3DXVECTOR3 m_extent;

    // row 0 : x, y and z of axis 0
    // row 1 : x, y and z of axis 1
    // row 2 : x, y and z of axis 2
    // row 3 : x, y and z of translation
    D3DXMATRIX m_transformationMatrix;

    vector<D3DXVECTOR3> m_vertices;
    LPD3DXEFFECT pEffect;

public:
    static vector<WORD> s_indices;

public:
    BoxCollider(IObject* pOwner);
    virtual ~BoxCollider();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void Init(const D3DXMATRIX& transform);

    void Update(const D3DXMATRIX& transform);

    void Render();

    const D3DXVECTOR3& GetExtent() const;
    const D3DXMATRIX&  GetTransformationMatrix() const;
};
