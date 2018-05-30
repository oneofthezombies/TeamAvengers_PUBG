#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"

CollisionManager::CollisionManager()
    : m_bIsRender(true)
{
}

bool CollisionManager::HasCollision(ColliderBase& lhs, ColliderBase& rhs)
{
    const ColliderBase::Type t1 = lhs.GetType();
    const ColliderBase::Type t2 = rhs.GetType();

    if (t1 == ColliderBase::Type::kBox &&
        t2 == ColliderBase::Type::kBox)
    {
        BoxCollider* b1 = static_cast<BoxCollider*>(&lhs);
        BoxCollider* b2 = static_cast<BoxCollider*>(&rhs);

        return HasCollision(*b1, *b2);
    }
    else if (t1 == ColliderBase::Type::kBox &&
             t2 == ColliderBase::Type::kSphere)
    {
        BoxCollider* b1 = static_cast<BoxCollider*>(&lhs);
        SphereCollider* s2 = static_cast<SphereCollider*>(&rhs);

        return HasCollision(*b1, *s2);
    }
    else if (t1 == ColliderBase::Type::kSphere &&
             t2 == ColliderBase::Type::kBox)
    {
        SphereCollider* s1 = static_cast<SphereCollider*>(&lhs);
        BoxCollider* b2 = static_cast<BoxCollider*>(&rhs);

        return HasCollision(*s1, *b2);
    }
    else if (t1 == ColliderBase::Type::kSphere &&
             t2 == ColliderBase::Type::kSphere)
    {
        SphereCollider* s1 = static_cast<SphereCollider*>(&lhs);
        SphereCollider* s2 = static_cast<SphereCollider*>(&rhs);

        return HasCollision(*s1, *s2);
    }
    else
    {
        // something error

        return false;
    }

    return false;
}

bool CollisionManager::HasCollision(const BoxCollider& lhs, const BoxCollider& rhs)
{
    const D3DXMATRIX& A_transform = lhs.GetTransform();
    const D3DXMATRIX& B_transform = rhs.GetTransform();

    const D3DXVECTOR3& A_extent = lhs.GetExtent();
    const D3DXVECTOR3& B_extent = rhs.GetExtent();

    const D3DXVECTOR3 distance = rhs.GetCenter() - lhs.GetCenter();

    vector<D3DXVECTOR3> A_axises(3);
    vector<D3DXVECTOR3> B_axises(3);
    vector<vector<float>> dotAB(3, vector<float>(3));
    vector<vector<float>> absDotAB(3, vector<float>(3));
    vector<float> dotAD(3);
    float r0, r1, r01, r;

    // a0
    A_axises[0].x = A_transform.m[0][0];
    A_axises[0].y = A_transform.m[0][1];
    A_axises[0].z = A_transform.m[0][2];
    B_axises[0].x = B_transform.m[0][0];
    B_axises[0].y = B_transform.m[0][1];
    B_axises[0].z = B_transform.m[0][2];
    B_axises[1].x = B_transform.m[1][0];
    B_axises[1].y = B_transform.m[1][1];
    B_axises[1].z = B_transform.m[1][2];
    B_axises[2].x = B_transform.m[2][0];
    B_axises[2].y = B_transform.m[2][1];
    B_axises[2].z = B_transform.m[2][2];
    dotAB[0][0] = D3DXVec3Dot(&A_axises[0], &B_axises[0]);
    dotAB[0][1] = D3DXVec3Dot(&A_axises[0], &B_axises[1]);
    dotAB[0][2] = D3DXVec3Dot(&A_axises[0], &B_axises[2]);
    dotAD[0] = D3DXVec3Dot(&A_axises[0], &distance);
    absDotAB[0][0] = abs(dotAB[0][0]);
    absDotAB[0][1] = abs(dotAB[0][1]);
    absDotAB[0][2] = abs(dotAB[0][2]);
    r = abs(dotAD[0]);
    r1 = B_extent.x * absDotAB[0][0] 
       + B_extent.y * absDotAB[0][1] 
       + B_extent.z * absDotAB[0][2];
    r01 = A_extent.x + r1;
    if (r > r01)
        return false;

    // a1
    A_axises[1].x = A_transform.m[1][0];
    A_axises[1].y = A_transform.m[1][1];
    A_axises[1].z = A_transform.m[1][2];
    dotAB[1][0] = D3DXVec3Dot(&A_axises[1], &B_axises[0]);
    dotAB[1][1] = D3DXVec3Dot(&A_axises[1], &B_axises[1]);
    dotAB[1][2] = D3DXVec3Dot(&A_axises[1], &B_axises[2]);
    dotAD[1] = D3DXVec3Dot(&A_axises[1], &distance);
    absDotAB[1][0] = abs(dotAB[1][0]);
    absDotAB[1][1] = abs(dotAB[1][1]);
    absDotAB[1][2] = abs(dotAB[1][2]);
    r = abs(dotAD[1]);
    r1 = B_extent.x * absDotAB[1][0] 
       + B_extent.y * absDotAB[1][1] 
       + B_extent.z * absDotAB[1][2];
    r01 = A_extent.y + r1;
    if (r > r01)
        return false;

    // a2
    A_axises[2].x = A_transform.m[2][0];
    A_axises[2].y = A_transform.m[2][1];
    A_axises[2].z = A_transform.m[2][2];
    dotAB[2][0] = D3DXVec3Dot(&A_axises[2], &B_axises[0]);
    dotAB[2][1] = D3DXVec3Dot(&A_axises[2], &B_axises[1]);
    dotAB[2][2] = D3DXVec3Dot(&A_axises[2], &B_axises[2]);
    dotAD[2] = D3DXVec3Dot(&A_axises[2], &distance);
    absDotAB[2][0] = abs(dotAB[2][0]);
    absDotAB[2][1] = abs(dotAB[2][1]);
    absDotAB[2][2] = abs(dotAB[2][2]);
    r = abs(dotAD[2]);
    r1 = B_extent.x * absDotAB[2][0]
       + B_extent.y * absDotAB[2][1] 
       + B_extent.z * absDotAB[2][2];
    r01 = A_extent.z + r1;
    if (r > r01)
        return false;

    // b0
    r = abs(D3DXVec3Dot(&B_axises[0], &distance));
    r0 = A_extent.x * absDotAB[0][0] 
       + A_extent.y * absDotAB[1][0] 
       + A_extent.z * absDotAB[2][0];
    r01 = r0 + B_extent.x;
    if (r > r01)
        return false;

    // b1
    r = abs(D3DXVec3Dot(&B_axises[1], &distance));
    r0 = A_extent.x * absDotAB[0][1] 
       + A_extent.y * absDotAB[1][1] 
       + A_extent.z * absDotAB[2][1];
    r01 = r0 + B_extent.y;
    if (r > r01)
        return false;

    // b2
    r = abs(D3DXVec3Dot(&B_axises[2], &distance));
    r0 = A_extent.x * absDotAB[0][2] 
       + A_extent.y * absDotAB[1][2] 
       + A_extent.z * absDotAB[2][2];
    r01 = r0 + B_extent.z;
    if (r > r01)
        return false;

    // a0 x b0
    r = abs(dotAD[2] * dotAB[1][0] - dotAD[1] * dotAB[2][0]);
    r0 = A_extent.y * absDotAB[2][0] 
       + A_extent.z * absDotAB[1][0];
    r1 = B_extent.y * absDotAB[0][2] 
       + B_extent.z * absDotAB[0][1];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a0 x b1
    r = abs(dotAD[2] * dotAB[1][1] - dotAD[1] * dotAB[2][1]);
    r0 = A_extent.y * absDotAB[2][1] 
       + A_extent.z * absDotAB[1][1];
    r1 = B_extent.x * absDotAB[0][2] 
       + B_extent.z * absDotAB[0][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a0 x b2
    r = abs(dotAD[2] * dotAB[1][2] - dotAD[1] * dotAB[2][2]);
    r0 = A_extent.y * absDotAB[2][2]
       + A_extent.z * absDotAB[1][2];
    r1 = B_extent.x * absDotAB[0][1]
       + B_extent.y * absDotAB[0][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a1 x b0
    r = abs(dotAD[0] * dotAB[2][0] - dotAD[2] * dotAB[0][0]);
    r0 = A_extent.x * absDotAB[2][0]
       + A_extent.z * absDotAB[0][0];
    r1 = B_extent.y * absDotAB[1][2]
       + B_extent.z * absDotAB[1][1];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a1 x b1
    r = abs(dotAD[0] * dotAB[2][1] - dotAD[2] * dotAB[0][1]);
    r0 = A_extent.x * absDotAB[2][1]
       + A_extent.z * absDotAB[0][1];
    r1 = B_extent.x * absDotAB[1][2]
       + B_extent.z * absDotAB[1][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a1 x b2
    r = abs(dotAD[0] * dotAB[2][2] - dotAD[2] * dotAB[0][2]);
    r0 = A_extent.x * absDotAB[2][2]
       + A_extent.z * absDotAB[0][2];
    r1 = B_extent.x * absDotAB[1][1]
       + B_extent.y * absDotAB[1][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a2 x b0
    r = abs(dotAD[1] * dotAB[0][0] - dotAD[0] * dotAB[1][0]);
    r0 = A_extent.x * absDotAB[1][0]
       + A_extent.y * absDotAB[0][0];
    r1 = B_extent.y * absDotAB[2][2]
       + B_extent.z * absDotAB[2][1];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a2 x b1
    r = abs(dotAD[1] * dotAB[0][1] - dotAD[0] * dotAB[1][1]);
    r0 = A_extent.x * absDotAB[1][1]
       + A_extent.y * absDotAB[0][1];
    r1 = B_extent.x * absDotAB[2][2]
       + B_extent.z * absDotAB[2][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a2 x b2
    r = abs(dotAD[1] * dotAB[0][2] - dotAD[0] * dotAB[1][2]);
    r0 = A_extent.x * absDotAB[1][2]
       + A_extent.y * absDotAB[0][2];
    r1 = B_extent.x * absDotAB[2][1]
       + B_extent.y * absDotAB[2][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    return true;
}

bool CollisionManager::HasCollision(const BoxCollider& lhs, const SphereCollider& rhs)
{
    return false;
}

bool CollisionManager::HasCollision(const SphereCollider& lhs, const BoxCollider& rhs)
{
    return false;
}

bool CollisionManager::HasCollision(const SphereCollider& lhs, const SphereCollider& rhs)
{
    return false;
}

void CollisionManager::Init()
{
}

void CollisionManager::Destroy()
{
}

void CollisionManager::Update()
{
    NotifyCollision();
}

void CollisionManager::Render()
{
    if (!m_bIsRender) return;

    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    for (auto c : m_usetColliderBases)
        SAFE_RENDER(c);

    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

void CollisionManager::AddColliderBase(ColliderBase& val)
{
    m_usetColliderBases.emplace(&val);
}

void CollisionManager::RemoveColliderBase(ColliderBase& val)
{
    m_usetColliderBases.erase(&val);
}

void CollisionManager::NotifyCollision()
{
    vector<ColliderBase*> perpetrators;
    vector<ColliderBase*> victims;

    for (auto& pvs : m_umapCollisionRelations)
    {
        const auto& perpetratorType = pvs.first;
        const auto& victimTypes = pvs.second;

        FindCollidersWithTag(perpetrators, perpetratorType);

        for (auto& victimType : victimTypes)
        {
            FindCollidersWithTag(victims, victimType);

            NotifyCollision(perpetrators, victims);
        }
    }
}

void CollisionManager::FindCollidersWithTag(vector<ColliderBase*>& OutColliders, const CollisionTag tag)
{
    OutColliders.resize(0);

    for (auto& c : m_usetColliderBases)
        if (c->GetTag() == tag)
            OutColliders.emplace_back(c);
}

void CollisionManager::NotifyCollision(const vector<ColliderBase*>& perpetrators, const vector<ColliderBase*>& victims)
{
    for (auto& perpetrator : perpetrators)
    {
        if (!perpetrator) continue;

        for (auto& victim : victims)
        {
            if (!victim) continue;
            if (perpetrator == victim) continue;

            NotifyCollision(perpetrator, victim);
        }
    }
}

void CollisionManager::NotifyCollision(ColliderBase* perpetrator, ColliderBase* victim)
{
    const auto search = m_usetPrevCollisions.find(make_pair(perpetrator, victim));
    if (HasCollision(*perpetrator, *victim))
    {
        if (search == m_usetPrevCollisions.end())
        {
            m_usetPrevCollisions.emplace(make_pair(perpetrator, victim));

            if (ICollisionListener* l2 = victim->GetListener())
                l2->OnCollisionEnter(*perpetrator);
        }
        else
        {
            if (ICollisionListener* l2 = victim->GetListener())
                l2->OnCollisionStay(*perpetrator);
        }
    }
    else
    {
        if (search != m_usetPrevCollisions.end())
        {
            m_usetPrevCollisions.erase(search);

            if (ICollisionListener* l2 = victim->GetListener())
                l2->OnCollisionExit(*perpetrator);
        }
    }
}

void CollisionManager::SetIsRender(const bool val)
{
    m_bIsRender = val;
}

void CollisionManager::SubscribeCollisionEvent(const CollisionTag perpetrator, const CollisionTag victim)
{
    auto& victims = m_umapCollisionRelations[perpetrator];
    for (auto& ct : victims)
        if (ct == victim) return;

    m_umapCollisionRelations[perpetrator].emplace_back(victim);
}

void CollisionManager::GetCollideds(vector<ColliderBase*>& OutCollideds, ColliderBase& perpetrator, const CollisionTag victim)
{
    OutCollideds.resize(0);

    vector<ColliderBase*> victims;
    FindCollidersWithTag(victims, victim);

    for (auto& v : victims)
        if (HasCollision(perpetrator, *v))
            OutCollideds.emplace_back(v);
}
