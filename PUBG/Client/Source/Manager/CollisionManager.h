#pragma once
#include "Singleton.h"
#include "TagClientOnly.h"

#define g_pCollision Collision()()

class Collider;
class BoxCollider;
class SphereCollider;

class CollisionManager : public Singleton<CollisionManager>
{
private:
    struct HashCollider
    {
        size_t operator()(const pair<Collider*, Collider*>& p) const
        {
            return hash<Collider*>{}(p.first) ^ hash<Collider*>{}(p.second);
        }
    };

    unordered_set<Collider*> colliders;
    unordered_set<pair<Collider*, Collider*>, HashCollider> 
        m_prevCollisions;
    unordered_map<TAG_COLLISION, unordered_set<TAG_COLLISION>> 
        m_collisionRelations;
    bool m_isRender;

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

    void SetIsRender(const bool val);
    bool IsRender() const;

    friend Singleton<CollisionManager>;
};

struct Collision
{
    CollisionManager* operator()()
    {
        return CollisionManager::GetInstance();
    }
};