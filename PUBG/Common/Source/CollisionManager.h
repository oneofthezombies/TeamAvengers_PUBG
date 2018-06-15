#pragma once
#include "Singleton.h"
#include "TagCollision.h"

#define g_pCollisionManager CollisionManager::GetInstance()

class Collider;
class BoxCollider;
class SphereCollider;
class ICollisionListener;

class CollisionManager : public Singleton<CollisionManager>
{
private:
    struct HashColliderBase
    {
        size_t operator()(const pair<Collider*, Collider*>& p) const
        {
            return hash<Collider*>{}(p.first) ^ hash<Collider*>{}(p.second);
        }
    };

    unordered_set<Collider*>                                    m_usetColliderBases;
    unordered_set<pair<Collider*, Collider*>, HashColliderBase> m_usetPrevCollisions;
    unordered_map<TAG_COLLISION, deque<TAG_COLLISION>>          m_umapCollisionRelations;

    CollisionManager();
    virtual ~CollisionManager() = default;

    bool HasCollision(Collider& lhs, Collider& rhs);
    bool HasCollision(const BoxCollider& lhs, const BoxCollider& rhs);

    // no impl
    bool HasCollision(const BoxCollider& lhs, const SphereCollider& rhs);

    // no impl
    bool HasCollision(const SphereCollider& lhs, const BoxCollider& rhs);

    // no impl
    bool HasCollision(const SphereCollider& lhs, const SphereCollider& rhs);

    void NotifyCollision();
    void NotifyCollision(const vector<Collider*>& perpetrators, const vector<Collider*>& victims);
    void NotifyCollision(Collider* perpetrator, Collider* victim);
    void FindCollidersWithTag(vector<Collider*>& OutColliders, const TAG_COLLISION tag);

public:
    void Init();
    void Destroy();
    void Update();

    void AddCollider(Collider* p);
    void RemoveCollider(Collider* p);

    void SubscribeCollisionEvent(const TAG_COLLISION perpetrator, const TAG_COLLISION victim);
    void GetCollideds(vector<Collider*>& OutCollideds, Collider& perpetrator, const TAG_COLLISION victim);

    friend Singleton<CollisionManager>;
};