#include "stdafx.h"
#include "Collision.h"
#include "Collider.h"

Collision::Manager::Manager()
    : Singleton<Collision::Manager>()
    , m_isRender(true)
{
}

bool Collision::Manager::hasCollision(Collider* pLhs, Collider* pRhs)
{
    assert(
        pLhs && 
        pRhs &&
        "Collision::Manager::hasCollision(Collider*, Collider*), \
         argument is null.");

    const Collider::Type t1 = pLhs->GetTypeCollider();
    const Collider::Type t2 = pRhs->GetTypeCollider();

    if (t1 == Collider::Type::BOX &&
        t2 == Collider::Type::BOX)
    {
        BoxCollider* b1 = static_cast<BoxCollider*>(pLhs);
        BoxCollider* b2 = static_cast<BoxCollider*>(pRhs);

        return hasCollision(b1, b2);
    }
    else if (
        t1 == Collider::Type::BOX &&
        t2 == Collider::Type::SPHERE)
    {
           BoxCollider* b1 = static_cast<   BoxCollider*>(pLhs);
        SphereCollider* s2 = static_cast<SphereCollider*>(pRhs);

        return hasCollision(b1, s2);
    }
    else if (
        t1 == Collider::Type::SPHERE &&
        t2 == Collider::Type::BOX)
    {
        SphereCollider* s1 = static_cast<SphereCollider*>(pLhs);
           BoxCollider* b2 = static_cast<   BoxCollider*>(pRhs);

        return hasCollision(s1, b2);
    }
    else if (
        t1 == Collider::Type::SPHERE &&
        t2 == Collider::Type::SPHERE)
    {
        SphereCollider* s1 = static_cast<SphereCollider*>(pLhs);
        SphereCollider* s2 = static_cast<SphereCollider*>(pRhs);

        return hasCollision(s1, s2);
    }
    else
    {
        assert(
            false && 
            "Collision::Manager::hasCollision(Collider*, Collider*), \
             it is an impossible case.");

        return false;
    }

    return false;
}

bool Collision::Manager::hasCollision(BoxCollider* pLhs, BoxCollider* pRhs)
{
    assert(
        pLhs && 
        pRhs && 
        "Collision::Manager::hasCollision(BoxCollider*, BoxCollider*), \
         argument is null.");

    return Core::HasCollision(pLhs, pRhs);
}

bool Collision::Manager::hasCollision(
    SphereCollider* pLhs, 
    SphereCollider* pRhs)
{
    assert(
        false && 
        "Collision::Manager::hasCollision(SphereCollider*, SphereCollider*), \
         no impl");

    return false;
}

bool Collision::Manager::hasCollision(SphereCollider* pLhs, BoxCollider* pRhs)
{
    assert(
        false &&
        "Collision::Manager::hasCollision(SphereCollider*, BoxCollider*), \
         no impl");

    return false;
}

bool Collision::Manager::hasCollision(BoxCollider* pLhs, SphereCollider* pRhs)
{
    assert(
        false &&
        "Collision::Manager::hasCollision(BoxCollider*, SphereCollider*), \
         no impl");

    return false;
}

void Collision::Manager::notifyCollision()
{
    std::vector<Collider*> offences;
    std::vector<Collider*> defences;

    for (auto& ods : m_collisionRelations)
    {
        const TAG_COLLISION& tagOffence = ods.first;
        const std::unordered_set<TAG_COLLISION>& tagDefences = ods.second;

        findCollidersWithTag(tagOffence, &offences);

        for (auto& tagDefence : tagDefences)
        {
            findCollidersWithTagInsideFrustum(tagDefence, &defences);

            notifyCollision(offences, defences);
        }
    }
}

void Collision::Manager::notifyCollision(
    const std::vector<Collider*>& offences, 
    const std::vector<Collider*>& defences)
{
    for (auto& pOffence : offences)
    {
        assert(
            pOffence &&
            "Collision::Manager::notifyCollision( \
                const std::vector<Collider*>&, \
                const std::vector<Collider*>&), \
             collider is null.");

        for (auto& pDefence : defences)
        {
            assert(pDefence &&
                "Collision::Manager::notifyCollision( \
                    const std::vector<Collider*>&, \
                    const std::vector<Collider*>&), \
                 collider is null.");

            if (pOffence == pDefence) continue;

            notifyCollision(pOffence, pDefence);
        }
    }
}

void Collision::Manager::notifyCollision(
    Collider* pOffence, 
    Collider* pDefence)
{
    assert(
        pOffence && 
        pDefence &&
        "Collision::Manager::notifyCollision( \
            Collider*, \
            Collider*), \
         collider is null.");

    const auto search =
        m_prevCollisions.find(std::make_pair(pOffence, pDefence));

    if (hasCollision(pOffence, pDefence))
    {
        if (search == m_prevCollisions.end())
        {
            for (auto& c : pDefence->GetOnCollisionEnterCallbacks())
                if (c) c(pOffence, pDefence);

            m_prevCollisions.emplace(std::make_pair(pOffence, pDefence));
        }
        else
        {
            for (auto& c : pDefence->GetOnCollisionStayCallbacks())
                if (c) c(pOffence, pDefence);
        }
    }
    else
    {
        if (search != m_prevCollisions.end())
        {
            for (auto& c : pDefence->GetOnCollisionExitCallbacks())
                if (c) c(pOffence, pDefence);

            m_prevCollisions.erase(search);
        }
    }
}

void Collision::Manager::findCollidersWithTag(
    const TAG_COLLISION           tag, 
          std::vector<Collider*>* OutColliders)
{
    assert(
        OutColliders &&
        "Collision::Manager::findCollidersWithTag(), colliders is null.");

    OutColliders->resize(0);

    for (auto& c : m_colliders)
    {
        assert(
            c && 
            "Collision::Manager::findCollidersWithTag(), collider is null.");

        if (c->GetTagCollision() == tag)
            OutColliders->emplace_back(c);
    }
}

void Collision::Manager::findCollidersWithTagInsideFrustum(
    const TAG_COLLISION           tag, 
          std::vector<Collider*>* OutColliders)
{
    assert(
        OutColliders &&
        "Collision::Manager::findCollidersWithTagInsideFrustum(), \
         colliders is null.");

    findCollidersWithTag(tag, OutColliders);
}

void Collision::Manager::Update()
{
    notifyCollision();
}

void Collision::Manager::AddCollider(Collider* pCollider)
{
    assert(
        pCollider && 
        "Collision::Manager::AddCollider(), collider is null.");

    m_colliders.emplace(pCollider);
}

void Collision::Manager::RemoveCollider(Collider* pCollider)
{
    assert(
        pCollider &&
        "Collision::Manager::RemoveCollider(), collider is null.");

    m_colliders.erase(pCollider);
}

void Collision::Manager::AddCollisionEvent(
    const TAG_COLLISION tagOffence, 
    const TAG_COLLISION tagDefence)
{
    m_collisionRelations[tagOffence].emplace(tagDefence);
}

void Collision::Manager::Simulate(
          Collider*               pOffence, 
    const TAG_COLLISION           tagDefence, 
          std::vector<Collider*>* OutDefences)
{
    assert(
        pOffence && 
        OutDefences && 
        "Collision::Manager::Simulate( \
            Collider*, \
            const TAG_COLLISION, \
            std::vector<Collider*>*), \
         argument is null.");

    OutDefences->resize(0);
    std::vector<Collider*> defences;

    findCollidersWithTagInsideFrustum(tagDefence, &defences);

    for (auto& d : defences)
    {
        assert(
            d &&
            "Collision::Manager::Simulate( \
            Collider*, \
            const TAG_COLLISION, \
            std::vector<Collider*>*), \
         collider is null.");

        if (hasCollision(pOffence, d))
            OutDefences->emplace_back(d);
    }
}

void Collision::Manager::Simulate(
    const TAG_COLLISION           tagOffence, 
          Collider*               pDefence, 
          std::vector<Collider*>* OutOffences)
{
    assert(
        pDefence &&
        OutOffences &&
        "Collision::Manager::Simulate( \
            const TAG_COLLISION, \
            Collider*, \
            std::vector<Collider*>*), \
         argument is null.");

    OutOffences->resize(0);
    std::vector<Collider*> offences;

    findCollidersWithTagInsideFrustum(tagOffence, &offences);

    for (auto& o : offences)
    {
        assert(
            o &&
            "Collision::Manager::Simulate( \
            const TAG_COLLISION, \
            Collider*, \
            std::vector<Collider*>*), \
         collider is null.");

        if (hasCollision(o, pDefence))
            OutOffences->emplace_back(o);
    }
}

void Collision::Manager::SetIsRender(const bool val)
{
    m_isRender = val;
}

bool Collision::Manager::IsRender() const
{
    return m_isRender;
}

Collision::Manager* Collision::operator()()
{
    return Manager::GetInstance();
}
