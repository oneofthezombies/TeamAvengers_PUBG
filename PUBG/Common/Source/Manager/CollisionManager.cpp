#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "BoxCollision.h"

CollisionManager::CollisionManager()
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

    return BoxCollision::HasCollision(pLhs, pRhs);

    //const D3DXMATRIX& A_transform = pLhs->GetTransform();
    //const D3DXMATRIX& B_transform = pRhs->GetTransform();

    //const D3DXVECTOR3& A_extent = pLhs->GetExtent();
    //const D3DXVECTOR3& B_extent = pRhs->GetExtent();

    //const D3DXVECTOR3 distance = pLhs->GetCenter() - pRhs->GetCenter();

    //vector<D3DXVECTOR3> A_axises(3);
    //vector<D3DXVECTOR3> B_axises(3);
    //vector<vector<float>> dotAB(3, vector<float>(3));
    //vector<vector<float>> absDotAB(3, vector<float>(3));
    //vector<float> dotAD(3);
    //float r0, r1, r01, r;

    //// a0
    //A_axises[0].x = A_transform.m[0][0];
    //A_axises[0].y = A_transform.m[0][1];
    //A_axises[0].z = A_transform.m[0][2];
    //B_axises[0].x = B_transform.m[0][0];
    //B_axises[0].y = B_transform.m[0][1];
    //B_axises[0].z = B_transform.m[0][2];
    //B_axises[1].x = B_transform.m[1][0];
    //B_axises[1].y = B_transform.m[1][1];
    //B_axises[1].z = B_transform.m[1][2];
    //B_axises[2].x = B_transform.m[2][0];
    //B_axises[2].y = B_transform.m[2][1];
    //B_axises[2].z = B_transform.m[2][2];
    //dotAB[0][0] = D3DXVec3Dot(&A_axises[0], &B_axises[0]);
    //dotAB[0][1] = D3DXVec3Dot(&A_axises[0], &B_axises[1]);
    //dotAB[0][2] = D3DXVec3Dot(&A_axises[0], &B_axises[2]);
    //dotAD[0] = D3DXVec3Dot(&A_axises[0], &distance);
    //absDotAB[0][0] = abs(dotAB[0][0]);
    //absDotAB[0][1] = abs(dotAB[0][1]);
    //absDotAB[0][2] = abs(dotAB[0][2]);
    //r = abs(dotAD[0]);
    //r1 = B_extent.x * absDotAB[0][0] 
    //   + B_extent.y * absDotAB[0][1] 
    //   + B_extent.z * absDotAB[0][2];
    //r01 = A_extent.x + r1;
    //if (r > r01)
    //    return false;

    //// a1
    //A_axises[1].x = A_transform.m[1][0];
    //A_axises[1].y = A_transform.m[1][1];
    //A_axises[1].z = A_transform.m[1][2];
    //dotAB[1][0] = D3DXVec3Dot(&A_axises[1], &B_axises[0]);
    //dotAB[1][1] = D3DXVec3Dot(&A_axises[1], &B_axises[1]);
    //dotAB[1][2] = D3DXVec3Dot(&A_axises[1], &B_axises[2]);
    //dotAD[1] = D3DXVec3Dot(&A_axises[1], &distance);
    //absDotAB[1][0] = abs(dotAB[1][0]);
    //absDotAB[1][1] = abs(dotAB[1][1]);
    //absDotAB[1][2] = abs(dotAB[1][2]);
    //r = abs(dotAD[1]);
    //r1 = B_extent.x * absDotAB[1][0] 
    //   + B_extent.y * absDotAB[1][1] 
    //   + B_extent.z * absDotAB[1][2];
    //r01 = A_extent.y + r1;
    //if (r > r01)
    //    return false;

    //// a2
    //A_axises[2].x = A_transform.m[2][0];
    //A_axises[2].y = A_transform.m[2][1];
    //A_axises[2].z = A_transform.m[2][2];
    //dotAB[2][0] = D3DXVec3Dot(&A_axises[2], &B_axises[0]);
    //dotAB[2][1] = D3DXVec3Dot(&A_axises[2], &B_axises[1]);
    //dotAB[2][2] = D3DXVec3Dot(&A_axises[2], &B_axises[2]);
    //dotAD[2] = D3DXVec3Dot(&A_axises[2], &distance);
    //absDotAB[2][0] = abs(dotAB[2][0]);
    //absDotAB[2][1] = abs(dotAB[2][1]);
    //absDotAB[2][2] = abs(dotAB[2][2]);
    //r = abs(dotAD[2]);
    //r1 = B_extent.x * absDotAB[2][0]
    //   + B_extent.y * absDotAB[2][1] 
    //   + B_extent.z * absDotAB[2][2];
    //r01 = A_extent.z + r1;
    //if (r > r01)
    //    return false;

    //// b0
    //r = abs(D3DXVec3Dot(&B_axises[0], &distance));
    //r0 = A_extent.x * absDotAB[0][0] 
    //   + A_extent.y * absDotAB[1][0] 
    //   + A_extent.z * absDotAB[2][0];
    //r01 = r0 + B_extent.x;
    //if (r > r01)
    //    return false;

    //// b1
    //r = abs(D3DXVec3Dot(&B_axises[1], &distance));
    //r0 = A_extent.x * absDotAB[0][1] 
    //   + A_extent.y * absDotAB[1][1] 
    //   + A_extent.z * absDotAB[2][1];
    //r01 = r0 + B_extent.y;
    //if (r > r01)
    //    return false;

    //// b2
    //r = abs(D3DXVec3Dot(&B_axises[2], &distance));
    //r0 = A_extent.x * absDotAB[0][2] 
    //   + A_extent.y * absDotAB[1][2] 
    //   + A_extent.z * absDotAB[2][2];
    //r01 = r0 + B_extent.z;
    //if (r > r01)
    //    return false;

    //// a0 x b0
    //r = abs(dotAD[2] * dotAB[1][0] - dotAD[1] * dotAB[2][0]);
    //r0 = A_extent.y * absDotAB[2][0] 
    //   + A_extent.z * absDotAB[1][0];
    //r1 = B_extent.y * absDotAB[0][2] 
    //   + B_extent.z * absDotAB[0][1];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a0 x b1
    //r = abs(dotAD[2] * dotAB[1][1] - dotAD[1] * dotAB[2][1]);
    //r0 = A_extent.y * absDotAB[2][1] 
    //   + A_extent.z * absDotAB[1][1];
    //r1 = B_extent.x * absDotAB[0][2] 
    //   + B_extent.z * absDotAB[0][0];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a0 x b2
    //r = abs(dotAD[2] * dotAB[1][2] - dotAD[1] * dotAB[2][2]);
    //r0 = A_extent.y * absDotAB[2][2]
    //   + A_extent.z * absDotAB[1][2];
    //r1 = B_extent.x * absDotAB[0][1]
    //   + B_extent.y * absDotAB[0][0];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a1 x b0
    //r = abs(dotAD[0] * dotAB[2][0] - dotAD[2] * dotAB[0][0]);
    //r0 = A_extent.x * absDotAB[2][0]
    //   + A_extent.z * absDotAB[0][0];
    //r1 = B_extent.y * absDotAB[1][2]
    //   + B_extent.z * absDotAB[1][1];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a1 x b1
    //r = abs(dotAD[0] * dotAB[2][1] - dotAD[2] * dotAB[0][1]);
    //r0 = A_extent.x * absDotAB[2][1]
    //   + A_extent.z * absDotAB[0][1];
    //r1 = B_extent.x * absDotAB[1][2]
    //   + B_extent.z * absDotAB[1][0];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a1 x b2
    //r = abs(dotAD[0] * dotAB[2][2] - dotAD[2] * dotAB[0][2]);
    //r0 = A_extent.x * absDotAB[2][2]
    //   + A_extent.z * absDotAB[0][2];
    //r1 = B_extent.x * absDotAB[1][1]
    //   + B_extent.y * absDotAB[1][0];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a2 x b0
    //r = abs(dotAD[1] * dotAB[0][0] - dotAD[0] * dotAB[1][0]);
    //r0 = A_extent.x * absDotAB[1][0]
    //   + A_extent.y * absDotAB[0][0];
    //r1 = B_extent.y * absDotAB[2][2]
    //   + B_extent.z * absDotAB[2][1];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a2 x b1
    //r = abs(dotAD[1] * dotAB[0][1] - dotAD[0] * dotAB[1][1]);
    //r0 = A_extent.x * absDotAB[1][1]
    //   + A_extent.y * absDotAB[0][1];
    //r1 = B_extent.x * absDotAB[2][2]
    //   + B_extent.z * absDotAB[2][0];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //// a2 x b2
    //r = abs(dotAD[1] * dotAB[0][2] - dotAD[0] * dotAB[1][2]);
    //r0 = A_extent.x * absDotAB[1][2]
    //   + A_extent.y * absDotAB[0][2];
    //r1 = B_extent.x * absDotAB[2][1]
    //   + B_extent.y * absDotAB[2][0];
    //r01 = r0 + r1;
    //if (r > r01)
    //    return false;

    //return true;
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

    m_Colliders.emplace(p);
}

void CollisionManager::RemoveCollider(Collider* p)
{
    assert(p && 
        "CollisionManager::RemoveCollider() failed. collider is null.");

    m_Colliders.erase(p);
}

void CollisionManager::notifyCollision()
{
    vector<Collider*> perpetrators;
    vector<Collider*> victims;

    for (auto& pvs : m_CollisionRelations)
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

    for (auto& c : m_Colliders)
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
        m_PrevCollisions.find(make_pair(pPerpetrator, pVictim));
    
    if (hasCollision(pPerpetrator, pVictim))
    {
        if (search == m_PrevCollisions.end())
        {
            m_PrevCollisions.emplace(make_pair(pPerpetrator, pVictim));

            if (ICollisionListener* l2 = pVictim->GetListener())
                l2->OnCollisionEnter(pPerpetrator, pVictim);
        }
        else
        {
            if (ICollisionListener* l2 = pVictim->GetListener())
                l2->OnCollisionStay(pPerpetrator, pVictim);
        }
    }
    else
    {
        if (search != m_PrevCollisions.end())
        {
            m_PrevCollisions.erase(search);

            if (ICollisionListener* l2 = pVictim->GetListener())
                l2->OnCollisionExit(pPerpetrator, pVictim);
        }
    }
}

void CollisionManager::SubscribeCollisionEvent(const TAG_COLLISION perpetrator, 
    const TAG_COLLISION victim)
{
    m_CollisionRelations[perpetrator].emplace(victim);
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
