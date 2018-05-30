#pragma once

#include "SingletonBase.h"

#define g_pCollisionManager CollisionManager::GetInstance()

enum class CollisionTag
{
    kIdle,
    kFoo,
    kBar,
    kBullet,
    kEnemy,
    kItem,
    kPlayer,
    kItemPicker,
};

class ColliderBase;
class BoxCollider;
class SphereCollider;
class ICollisionListener;

class CollisionManager : public SingletonBase<CollisionManager>
{
private:
    struct HashColliderBase
    {
        size_t operator()(const pair<ColliderBase*, ColliderBase*>& p) const
        {
            return hash<ColliderBase*>{}(p.first) ^ hash<ColliderBase*>{}(p.second);
        }
    };

    unordered_set<ColliderBase*>                                        m_usetColliderBases;
    unordered_set<pair<ColliderBase*, ColliderBase*>, HashColliderBase> m_usetPrevCollisions;
    unordered_map<CollisionTag, deque<CollisionTag>>                    m_umapCollisionRelations;
    bool m_bIsRender;

    CollisionManager();
    virtual ~CollisionManager() = default;

    bool HasCollision(ColliderBase& lhs, ColliderBase& rhs);
    bool HasCollision(const BoxCollider& lhs, const BoxCollider& rhs);

    // no impl
    bool HasCollision(const BoxCollider& lhs, const SphereCollider& rhs);

    // no impl
    bool HasCollision(const SphereCollider& lhs, const BoxCollider& rhs);

    // no impl
    bool HasCollision(const SphereCollider& lhs, const SphereCollider& rhs);

    void NotifyCollision();
    void NotifyCollision(const vector<ColliderBase*>& perpetrators, const vector<ColliderBase*>& victims);
    void NotifyCollision(ColliderBase* perpetrator, ColliderBase* victim);
    void FindCollidersWithTag(vector<ColliderBase*>& OutColliders, const CollisionTag tag);

public:
    void Init();
    void Destroy();
    void Update();
    void Render();

    void AddColliderBase(ColliderBase& val);
    void RemoveColliderBase(ColliderBase& val);

    void SetIsRender(const bool val);

    void SubscribeCollisionEvent(const CollisionTag perpetrator, const CollisionTag victim);
    void GetCollideds(vector<ColliderBase*>& OutCollideds, ColliderBase& perpetrator, const CollisionTag victim);

    friend SingletonBase<CollisionManager>;
};