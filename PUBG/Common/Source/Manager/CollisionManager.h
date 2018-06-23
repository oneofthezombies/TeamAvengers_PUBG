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

    unordered_set<Collider*> m_Colliders;
    unordered_set<pair<Collider*, Collider*>, HashColliderBase> 
        m_PrevCollisions;
    unordered_map<TAG_COLLISION, unordered_set<TAG_COLLISION>> 
        m_CollisionRelations;

private:
    CollisionManager();
    virtual ~CollisionManager() = default;

    bool hasCollision(Collider* pLhs, Collider* pRhs);
    bool hasCollision(BoxCollider* pLhs, BoxCollider* pRhs);
    bool hasCollision(BoxCollider* pLhs, SphereCollider* pRhs);
    bool hasCollision(SphereCollider*pLhs, BoxCollider* pRhs);
    bool hasCollision(SphereCollider* pLhs, SphereCollider* pRhs);

    void notifyCollision();
    void notifyCollision(const vector<Collider*>& perpetrators, 
        const vector<Collider*>& victims);
    void notifyCollision(Collider* pPerpetrator, Collider* pVictim);
    void findCollidersWithTag(const TAG_COLLISION tag, 
        vector<Collider*>* OutColliders);

public:
    void Update();

    void AddCollider(Collider* p);
    void RemoveCollider(Collider* p);

    void SubscribeCollisionEvent(const TAG_COLLISION perpetrator, 
        const TAG_COLLISION victim);
    void GetCollideds(Collider* pPerpetrator, const TAG_COLLISION victim, 
        vector<Collider*>* OutCollideds);

    friend Singleton<CollisionManager>;
};