#pragma once
#include "Singleton.h"
#include "TagClientOnly.h"

#define g_pCollision Collision()()

struct BoundingShape
{
    D3DXVECTOR3 center;
    D3DXVECTOR3 position;
    bool        isRender;

             BoundingShape();
    virtual ~BoundingShape();
};

struct BoundingSphere : public BoundingShape
{
    float       radius;

             BoundingSphere();
    virtual ~BoundingSphere();

    void Render();
    void RenderRed();

    static BoundingSphere Create(const D3DXVECTOR3& position,const float radius);
};

struct BoundingBox : public BoundingShape
{
    D3DXVECTOR3 extent;
    D3DXQUATERNION rotation;

             BoundingBox();
    virtual ~BoundingBox();

    void Render();
    void RenderRed();

    static BoundingBox Create(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    static BoundingBox Create(const D3DXMATRIX& transformationMatrix);
};

struct BoundingRect
{
    D3DXVECTOR2 center;
    D3DXVECTOR2 position;
    D3DXVECTOR2 extent;
    //D3DXVECTOR2 rotation;

             BoundingRect();
    virtual ~BoundingRect();
};

class Collider;
class BoxCollider;
class SphereCollider;
struct Collision;
struct HashColliderPair;

using colliders_t      = std::unordered_set<Collider*>;
using prev_collision_t = std::pair<Collider*, Collider*>;

using prev_collisions_t =
    std::unordered_set<prev_collision_t, HashColliderPair>;

using collision_relations_t = 
    std::unordered_map<TAG_COLLISION, std::unordered_set<TAG_COLLISION>>;

struct HashColliderPair
{
    std::size_t operator()(const std::pair<Collider*, Collider*>& p) const
    {
        return std::hash<Collider*>()(p.first)
             ^ std::hash<Collider*>()(p.second);
    }
};

struct Collision
{
    struct Core
    {
        static bool HasCollision(BoxCollider* pLhs, BoxCollider* pRhs);
    };

    class Manager : public Singleton<Manager>
    {
    private:
        colliders_t           m_colliders;
        prev_collisions_t     m_prevCollisions;
        collision_relations_t m_collisionRelations;
        bool                  m_isRender;
        
    
    private:

                 Manager();
        virtual ~Manager() = default;

        bool hasCollision(      Collider* pLhs,       Collider* pRhs);
        bool hasCollision(   BoxCollider* pLhs,    BoxCollider* pRhs);

        // TODO : impl
        bool hasCollision(SphereCollider* pLhs, SphereCollider* pRhs);
        bool hasCollision(SphereCollider* pLhs,    BoxCollider* pRhs);
        bool hasCollision(   BoxCollider* pLhs, SphereCollider* pRhs);

        void notifyCollision();
        void notifyCollision(
            const std::vector<Collider*>& offences,
            const std::vector<Collider*>& defences);

        void notifyCollision(Collider* pOffence, Collider* pDefence);

        void findCollidersWithTag(
            const TAG_COLLISION           tag, 
                  std::vector<Collider*>* OutColliders);

        // TODO : add IsInsideFrustum
        void findCollidersWithTagInsideFrustum(
            const TAG_COLLISION           tag,
                  std::vector<Collider*>* OutColliders);

    public:
        void Update();

        void AddCollider   (Collider* pCollider);
        void RemoveCollider(Collider* pCollider);

        void AddCollisionEvent(
            const TAG_COLLISION tagOffence, 
            const TAG_COLLISION tagDefence);

        void Simulate(
                  Collider*               pOffence,
            const TAG_COLLISION           tagDefence,
                  std::vector<Collider*>* OutDefences);

        void Simulate(
            const TAG_COLLISION           tagOffence,
                  Collider*               pDefence,
                  std::vector<Collider*>* OutOffences);

        void SetIsRender(const bool val);
        bool IsRender() const;

        friend Singleton<Manager>;
    };

    Manager* operator()();

    static bool HasCollision(
        const BoundingSphere& lhs,
        const BoundingSphere& rhs);

    static bool HasCollision(const BoundingBox& lhs, const BoundingBox& rhs);
    static bool HasCollision(
        const Ray& ray, 
        const BoundingBox& box, 
        float* OutDistance);
    static bool HasCollision(
        const Ray& ray,
        const BoundingBox& box,
         const float rayEnd);
    static bool HasCollision(const Ray& ray, const BoundingRect& rect);
    static bool HasCollision(const Ray& ray, const BoundingRect& rect,const float end);
    static bool HasCollision(const BoundingSphere& sphere, const BoundingBox& box);
    static bool HasCollision2(const BoundingSphere& sphere, const BoundingBox& box);
    static std::vector<D3DXVECTOR3> GetCollidedNormal(const D3DXVECTOR3& mypos, const BoundingBox& box);
    static D3DXVECTOR3 GetCollidedNormal2(const D3DXVECTOR3& mypos, const BoundingBox& box);
};

