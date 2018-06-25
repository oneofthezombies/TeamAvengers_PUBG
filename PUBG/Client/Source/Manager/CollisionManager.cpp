#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CollisionCore.h"

CollisionManager::CollisionManager()
    : Singleton<CollisionManager>()
{
}

bool CollisionManager::hasCollision(Collider* pLhs, Collider* pRhs)
{
    assert(pLhs && pRhs && 
        "CollisionManager::HasCollision() failed. collider is null.");

    const Collider::TYPE t1 = pLhs->GetType();
    const Collider::TYPE t2 = pRhs->GetType();

    if (t1 == Collider::TYPE::BOX &&
        t2 == Collider::TYPE::BOX)
    {
        BoxCollider* b1 = static_cast<BoxCollider*>(pLhs);
        BoxCollider* b2 = static_cast<BoxCollider*>(pRhs);

        return hasCollision(b1, b2);
    }
    else if (t1 == Collider::TYPE::BOX &&
             t2 == Collider::TYPE::SPHERE)
    {
        BoxCollider* b1 = static_cast<BoxCollider*>(pLhs);
        SphereCollider* s2 = static_cast<SphereCollider*>(pRhs);

        return hasCollision(b1, s2);
    }
    else if (t1 == Collider::TYPE::SPHERE &&
             t2 == Collider::TYPE::BOX)
    {
        SphereCollider* s1 = static_cast<SphereCollider*>(pLhs);
        BoxCollider* b2 = static_cast<BoxCollider*>(pRhs);

        return hasCollision(s1, b2);
    }
    else if (t1 == Collider::TYPE::SPHERE &&
             t2 == Collider::TYPE::SPHERE)
    {
        SphereCollider* s1 = static_cast<SphereCollider*>(pLhs);
        SphereCollider* s2 = static_cast<SphereCollider*>(pRhs);

        return hasCollision(s1, s2);
    }
    else
    {
        assert(false && "CollisionManager::HasCollision() failed. \
            it is an impossible case.");

        return false;
    }

    return false;
}

bool CollisionManager::hasCollision(BoxCollider* pLhs, BoxCollider* pRhs)
{
    assert(pLhs && pRhs && 
        "CollisionManager::HasCollision() failed. collider is null.");

    return CollisionCore::HasCollision(pLhs, pRhs);
}

bool CollisionManager::hasCollision(BoxCollider* pLhs, SphereCollider* pRhs)
{
    assert(false && 
        "CollisionManager::HasCollision(). box vs sphere is no impl.");

    return false;
}

bool CollisionManager::hasCollision(SphereCollider* pLhs, BoxCollider* pRhs)
{
    assert(false &&
        "CollisionManager::HasCollision(). sphere vs box is no impl.");

    return false;
}

bool CollisionManager::hasCollision(SphereCollider* pLhs, SphereCollider* pRhs)
{
    assert(false &&
        "CollisionManager::HasCollision(). sphere vs sphere is no impl.");

    return false;
}

void CollisionManager::Update()
{
    notifyCollision();
}

void CollisionManager::AddCollider(Collider* p)
{
    assert(p && "CollisionManager::AddCollider() failed. collider is null.");

    colliders.emplace(p);
}

void CollisionManager::RemoveCollider(Collider* p)
{
    assert(p && 
        "CollisionManager::RemoveCollider() failed. collider is null.");

    colliders.erase(p);
}

void CollisionManager::notifyCollision()
{
    vector<Collider*> perpetrators;
    vector<Collider*> victims;

    for (auto& pvs : m_collisionRelations)
    {
        const auto& perpetratorType = pvs.first;
        const auto& victimTypes = pvs.second;

        findCollidersWithTag(perpetratorType, &perpetrators);

        for (auto& victimType : victimTypes)
        {
            findCollidersWithTag(victimType, &victims);

            notifyCollision(perpetrators, victims);
        }
    }
}

void CollisionManager::findCollidersWithTag(const TAG_COLLISION tag,
    vector<Collider*>* OutColliders)
{
    assert(OutColliders && 
        "CollisionManager::FindCollidersWithTag() failed. colliders is null.");

    OutColliders->resize(0);

    for (auto& c : colliders)
        if (c->GetTag() == tag)
            OutColliders->emplace_back(c);
}

void CollisionManager::notifyCollision(const vector<Collider*>& perpetrators,
    const vector<Collider*>& victims)
{
    for (auto& pPerpetrator : perpetrators)
    {
        assert(pPerpetrator && 
            "CollisionManager::NotifyCollision() failed. collider is null.");

        for (auto& pVictim : victims)
        {
            assert(pPerpetrator &&
                "CollisionManager::NotifyCollision() failed. \
                 collider is null.");

            if (pPerpetrator == pVictim) continue;

            notifyCollision(pPerpetrator, pVictim);
        }
    }
}

void CollisionManager::notifyCollision(Collider* pPerpetrator,
    Collider* pVictim)
{
    assert(pPerpetrator && pVictim && 
        "CollisionManager::notifyCollision() failed. collider is null.");

    const auto search = 
        m_prevCollisions.find(make_pair(pPerpetrator, pVictim));
    
    if (hasCollision(pPerpetrator, pVictim))
    {
        if (search == m_prevCollisions.end())
        {
            for (auto& c : pVictim->GetOnCollisionEnterCallbacks())
                if (c) c(pPerpetrator, pVictim);

            m_prevCollisions.emplace(make_pair(pPerpetrator, pVictim));
        }
        else
        {
            for (auto& c : pVictim->GetOnCollisionStayCallbacks())
                if (c) c(pPerpetrator, pVictim);
        }
    }
    else
    {
        if (search != m_prevCollisions.end())
        {
            for (auto& c : pVictim->GetOnCollisionExitCallbacks())
                if (c) c(pPerpetrator, pVictim);

            m_prevCollisions.erase(search);
        }
    }
}

void CollisionManager::SubscribeCollisionEvent(const TAG_COLLISION perpetrator,
    const TAG_COLLISION victim)
{
    m_collisionRelations[perpetrator].emplace(victim);
}

void CollisionManager::GetCollideds(Collider* pPerpetrator,
    const TAG_COLLISION victim, vector<Collider*>* OutCollideds)
{
    assert(OutCollideds && 
        "CollisionManager::GetCollideds() failed. colliders is null.");

    OutCollideds->resize(0);

    vector<Collider*> victims;
    findCollidersWithTag(victim, &victims);

    for (auto& v : victims)
    {
        assert(v && 
            "CollisionManager::GetCollideds() failed. collider is null.");

        if (hasCollision(pPerpetrator, v))
            OutCollideds->emplace_back(v);
    }
}
