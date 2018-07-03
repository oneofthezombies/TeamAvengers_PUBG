#pragma once
#include "Singleton.h"
#include "TagClientOnly.h"

#define g_pCollision Collision()()

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
};

