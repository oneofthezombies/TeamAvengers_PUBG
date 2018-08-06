#pragma once
#include "ComponentTransform.h"
#include "TagClientOnly.h"

class IObject;
class Collider;

using on_collision_callback_t =
    std::function<void(Collider* pOffence, Collider* pDefence)>;
using on_collision_callbacks_t = std::deque<on_collision_callback_t>;

class Collider : public Component
{
public:
    enum class Type
    {
        BOX,
        SPHERE,
    };

private:
    Type                     m_typeCollider;
    TAG_COLLISION            m_tagCollision;
    on_collision_callbacks_t m_onCollisionEnterCallbacks;
    on_collision_callbacks_t m_onCollisionStayCallbacks;
    on_collision_callbacks_t m_onCollisionExitCallbacks;

protected:
    D3DXVECTOR3              m_center;
    D3DCOLOR                 m_color;

protected:
             Collider(IObject* pOwner, const Type type);

public:
    virtual ~Collider();

    Type               GetTypeCollider() const;
    const D3DXVECTOR3& GetCenter() const;

    void          SetTagCollision(const TAG_COLLISION tag);
    TAG_COLLISION GetTagCollision() const;

    void AddOnCollisionEnterCallback(const on_collision_callback_t& callback);
    void AddOnCollisionStayCallback (const on_collision_callback_t& callback);
    void AddOnCollisionExitCallback (const on_collision_callback_t& callback);

    const on_collision_callbacks_t& GetOnCollisionEnterCallbacks() const;
    const on_collision_callbacks_t& GetOnCollisionStayCallbacks() const;
    const on_collision_callbacks_t& GetOnCollisionExitCallbacks() const;
};

class SphereCollider : public Collider
{
private:
    float m_radius;

public:
             SphereCollider(IObject* pOwner);
    virtual ~SphereCollider() = default;

    void Init(const float radius);
    void Update(const D3DXMATRIX& transformationMatrix);
};

class BoxCollider : public Collider
{
public:
    static std::vector<WORD> INDICES;
    static std::vector<WORD> FRUSTUM_INDICES;

private:
    D3DXVECTOR3              m_extent;

    // row 0 : x, y and z of axis 0
    // row 1 : x, y and z of axis 1
    // row 2 : x, y and z of axis 2
    // row 3 : x, y and z of translation
    D3DXMATRIX               m_transformationMatrix;

    std::vector<D3DXVECTOR3> m_vertices;
    LPD3DXEFFECT             pEffect;

public:
             BoxCollider(IObject* pOwner);
    virtual ~BoxCollider();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void Init(const D3DXMATRIX& transformationMatrix);
    void Update(const D3DXMATRIX& transformationMatrix);
    void Render();

    const D3DXVECTOR3& GetExtent() const;
    const D3DXMATRIX&  GetTransformationMatrix() const;
};
