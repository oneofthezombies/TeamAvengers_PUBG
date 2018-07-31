#include "stdafx.h"
#include "Collision.h"
#include "Collider.h"

BoundingShape::BoundingShape()
    : /*MemoryAllocator()
    ,*/ center(Vector3::ZERO)
    , position(Vector3::ZERO)
    , isRender(false)
{
}

BoundingShape::~BoundingShape()
{
}

BoundingSphere::BoundingSphere()
    : BoundingShape()
    , radius(0.0f)

{
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::Render()
{
    if (!Collision()()->IsRender()) return;

    if (!isRender) return; // for recording

    D3DXMATRIX m;
    D3DXMatrixTransformation(
        &m, 
        nullptr, 
        nullptr, 
        &D3DXVECTOR3(radius, radius, radius), 
        nullptr, nullptr, 
        &(center + position));

    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        Resource()()->GetBoundingSphereMesh(),
        0,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR Green(0.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &Green, sizeof Green);
    });
    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
void BoundingSphere::RenderRed()
{
    if (!Collision()()->IsRender()) return;

    if (!isRender) return;

    D3DXMATRIX m;
    D3DXMatrixTransformation(
        &m,
        nullptr,
        nullptr,
        &D3DXVECTOR3(radius, radius, radius),
        nullptr, nullptr,
        &(center + position));

    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        Resource()()->GetBoundingSphereMesh(),
        0,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR Red(1.0f, 0.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &Red, sizeof Red);
    });
    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


}
void BoundingBox::RenderRed()
{
    if (!Collision()()->IsRender()) return;

    if (!isRender) return;

    auto& vertices = Resource()()->GetBoundingBoxVertices();
    auto& indices = Resource()()->GetBoundingBoxIndices();

    D3DXMATRIX e, c, r, p, m;
    D3DXMatrixScaling(&e, extent.x, extent.y, extent.z);
    D3DXMatrixTranslation(&c, center.x, center.y, center.z);
    D3DXMatrixRotationQuaternion(&r, &rotation);
    D3DXMatrixTranslation(&p, position.x, position.y, position.z);
    m = e * c * r * p;

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);
        D3DXCOLOR Red(1.0f, 0.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &Red, sizeof Red);
    },
        [&vertices, &indices]()
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            static_cast<UINT>(vertices.size()),
            static_cast<UINT>(indices.size() / 2),
            indices.data(),
            D3DFMT_INDEX16,
            vertices.data(),
            sizeof vertices.front());
    });

}


BoundingSphere BoundingSphere::Create(const D3DXVECTOR3& position, const float radius)
{
    BoundingSphere bs;
    bs.radius = radius;
    bs.position = position;
    return bs;
}

BoundingBox::BoundingBox()
    : BoundingShape()
    , extent(Vector3::ZERO)
    , rotation(Quaternion::IDENTITY)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Render()
{
    if (!Collision()()->IsRender()) return;

    if (!isRender) return; // for recording

    auto& vertices = Resource()()->GetBoundingBoxVertices();
    auto& indices  = Resource()()->GetBoundingBoxIndices();
    
    D3DXMATRIX e, c, r, p, m;
    D3DXMatrixScaling(&e, extent.x, extent.y, extent.z);
    D3DXMatrixTranslation(&c, center.x, center.y, center.z);
    D3DXMatrixRotationQuaternion(&r, &rotation);
    D3DXMatrixTranslation(&p, position.x, position.y, position.z);
    m = e * c * r * p;

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);
        D3DXCOLOR green(0.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &green, sizeof green);
    },
        [&vertices, &indices]()
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            static_cast<UINT>(vertices.size()),
            static_cast<UINT>(indices.size() / 2),
            indices.data(),
            D3DFMT_INDEX16,
            vertices.data(),
            sizeof vertices.front());
    });
}



BoundingBox BoundingBox::Create(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    BoundingBox bb;
    bb.center = (min + max) * 0.5f;
    bb.extent = max - bb.center;
    return bb;
}

BoundingBox BoundingBox::Create(const D3DXMATRIX& transformationMatrix)
{
    D3DXVECTOR3 extent(Vector3::ONE * 5.0f);
    D3DXVECTOR3 vS;
    D3DXQUATERNION qR;
    Matrix::GetScaleAndRotation(transformationMatrix, &vS, &qR);
    D3DXVECTOR3 vT = Matrix::GetTranslation(transformationMatrix);

    D3DXMATRIX mS;
    D3DXMatrixScaling(&mS, vS.x, vS.y, vS.z);
    D3DXVec3TransformCoord(&extent, &extent, &mS);

    BoundingBox bb = Create(-extent, extent);
    bb.rotation = qR;
    bb.position = vT;
    return bb;
}

Collision::Manager::Manager()
    : Singleton<Collision::Manager>()
    , m_isRender(false)
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
    // notifyBulletEvent()
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

bool Collision::HasCollision(
    const BoundingSphere& lhs,
    const BoundingSphere& rhs)
{
    return 
        D3DXVec3Length(&(
        (lhs.center + lhs.position) - (rhs.center + rhs.position))) 
        <= lhs.radius + rhs.radius;
}


bool Collision::HasCollision(const BoundingSphere& sphere, const BoundingBox& box)
{
    D3DXVECTOR3 spherePos = sphere.center + sphere.position;

    D3DXVECTOR3 boxMax = box.extent + box.center;
    D3DXVECTOR3 boxMin = -box.extent + box.center;

    D3DXVECTOR3 EightEdge[8];
    EightEdge[0] = D3DXVECTOR3(boxMax.x, boxMax.y, boxMax.z);
    EightEdge[1] = D3DXVECTOR3(boxMax.x, boxMax.y, boxMin.z);
    EightEdge[2] = D3DXVECTOR3(boxMax.x, boxMin.y, boxMax.z);
    EightEdge[3] = D3DXVECTOR3(boxMax.x, boxMin.y, boxMin.z);
    EightEdge[4] = D3DXVECTOR3(boxMin.x, boxMax.y, boxMax.z);
    EightEdge[5] = D3DXVECTOR3(boxMin.x, boxMax.y, boxMin.z);
    EightEdge[6] = D3DXVECTOR3(boxMin.x, boxMin.y, boxMax.z);
    EightEdge[7] = D3DXVECTOR3(boxMin.x, boxMin.y, boxMin.z);


    D3DXMATRIX B_transform;
    D3DXMatrixRotationQuaternion(&B_transform, &box.rotation);

    int find = -1;
    float min = FLT_MAX;
    for (int i = 0; i < 8; i++)
    {
        D3DXVec3TransformCoord(&EightEdge[i], &EightEdge[i], &B_transform);
        EightEdge[i] += box.position;

        float length = D3DXVec3Length(&(EightEdge[i] - spherePos));

        if (min > length)
        {
            min = length;
            find = i;
        }
    }


    //// get box closest point to sphere center by clamping
    //float x = std::max(boxMin.x, std::min(spherePos.x, boxMax.x));
    //float y = std::max(boxMin.y, std::min(spherePos.y, boxMax.y));
    //float z = std::max(boxMin.z, std::min(spherePos.z, boxMax.z));
    float x = EightEdge[find].x;
    float y = EightEdge[find].y;
    float z = EightEdge[find].z;

    // this is the same as isPointInsideSphere
    float distance = std::sqrt((x - spherePos.x) * (x - spherePos.x) +
        (y - spherePos.y) * (y - spherePos.y) +
        (z - spherePos.z) * (z - spherePos.z));

    return distance <= sphere.radius*sphere.radius;
}
float sqDistPointAABB(D3DXVECTOR3 p, BoundingBox aabb)
{
    float sqDist = 0.0f;
    D3DXVECTOR3 aabbMax = aabb.extent + aabb.center;
    D3DXVECTOR3 aabbMin = -aabb.extent + aabb.center;
    
    D3DXMATRIX B_transform;
    D3DXMatrixRotationQuaternion(&B_transform, &aabb.rotation);

    D3DXVec3TransformCoord(&aabbMax, &aabbMax, &B_transform);
    D3DXVec3TransformCoord(&aabbMin, &aabbMin, &B_transform);
    aabbMax += aabb.position;
    aabbMin += aabb.position;
    {//x
        float v = p.x;
        if (v < aabbMin.x) sqDist += (aabbMin.x - v)*(aabbMin.x - v);
        if (v > aabbMax.x) sqDist += (v - aabbMax.x)*(v - aabbMax.x);
    }
    {//y
        float v = p.y;
        if (v < aabbMin.y) sqDist += (aabbMin.y - v)*(aabbMin.y - v);
        if (v > aabbMax.y) sqDist += (v - aabbMax.y)*(v - aabbMax.y);
    }
    {//z
        float v = p.z;
        if (v < aabbMin.z) sqDist += (aabbMin.z - v)*(aabbMin.z - v);
        if (v > aabbMax.z) sqDist += (v - aabbMax.z)*(v - aabbMax.z);
    }
    return sqDist;
}

bool Collision::HasCollision2(const BoundingSphere & sphere, const BoundingBox & box)
{

    float sqDist = sqDistPointAABB((sphere.center+sphere.position), box);


    return sqDist <=sphere.radius*sphere.radius;
}

std::vector<D3DXVECTOR3> Collision::GetCollidedNormal(const D3DXVECTOR3& mypos, const BoundingBox& box)
{
    std::vector<D3DXVECTOR3> m_vecProj;
    m_vecProj.resize(8);
    m_vecProj[0] = (D3DXVECTOR3(-1, 1, 1));	    //ÁÂ»óÈÄ
    m_vecProj[1] = (D3DXVECTOR3(1, 1, 1));	    //¿ì»óÈÄ
    m_vecProj[2] = (D3DXVECTOR3(-1, 1, -1));	//ÁÂ»óÀü
    m_vecProj[3] = (D3DXVECTOR3(1, 1, -1));	    //¿ì»óÀü
    m_vecProj[4] = (D3DXVECTOR3(-1, -1, 1));	//ÁÂÇÏÈÄ
    m_vecProj[5] = (D3DXVECTOR3(1, -1, 1));	    //¿ìÇÏÈÄ
    m_vecProj[6] = (D3DXVECTOR3(-1, -1, -1));	//ÁÂÇÏÀü 
    m_vecProj[7] = (D3DXVECTOR3(1, -1, -1));	//¿ìÇÏÀü

    D3DXMATRIX s, r, t, m;
    D3DXMatrixScaling(&s, box.extent.x, box.extent.y, box.extent.z);
    D3DXMatrixRotationQuaternion(&r, &box.rotation);
    D3DXVECTOR3 boxPos(box.center + box.position);
    D3DXMatrixTranslation(&t, boxPos.x, boxPos.y, boxPos.z);
    m = s * r * t;

    std::vector<D3DXVECTOR3> m_vecWorld;
    m_vecWorld.resize(8);
    for (size_t i = 0; i < m_vecWorld.size(); ++i)
        D3DXVec3TransformCoord(&m_vecWorld[i], &m_vecProj[i], &m);

    std::vector<D3DXPLANE> m_vecPlane;
    m_vecPlane.resize(6);
    //±ÙÆò¸é//ÁÂ»óÀü//¿ì»óÀü//ÁÂÇÏÀü
    D3DXPlaneFromPoints(&m_vecPlane[0], &m_vecWorld[2], &m_vecWorld[3], &m_vecWorld[6]);
    //¿øÆò¸é//¿ì»óÈÄ//ÁÂ»óÈÄ//¿ìÇÏÈÄ
    D3DXPlaneFromPoints(&m_vecPlane[1], &m_vecWorld[1], &m_vecWorld[0], &m_vecWorld[5]);
    //ÁÂÆò¸é//ÁÂ»óÈÄ//ÁÂ»óÀü//ÁÂÇÏÈÄ
    D3DXPlaneFromPoints(&m_vecPlane[2], &m_vecWorld[0], &m_vecWorld[2], &m_vecWorld[4]);
    //¿ìÆò¸é//¿ì»óÀü//¿ì»óÈÄ//¿ìÇÏÀü
    D3DXPlaneFromPoints(&m_vecPlane[3], &m_vecWorld[3], &m_vecWorld[1], &m_vecWorld[7]);
    //»óÆò¸é//ÁÂ»óÈÄ//¿ì»óÈÄ//ÁÂ»óÀü
    D3DXPlaneFromPoints(&m_vecPlane[4], &m_vecWorld[0], &m_vecWorld[1], &m_vecWorld[2]);
    //ÇÏÆò¸é//ÁÂÇÏÀü//¿ìÇÏÀü//ÁÂÇÏÈÄ
    D3DXPlaneFromPoints(&m_vecPlane[5], &m_vecWorld[6], &m_vecWorld[7], &m_vecWorld[4]);

    D3DXVECTOR3 p12(boxPos - mypos);
    float p12len = D3DXVec3Length(&p12);
    std::vector<int> results;

    for (size_t i = 0; i < m_vecPlane.size(); ++i)
    {

        D3DXVECTOR3 pout;
        if (D3DXPlaneIntersectLine(&pout, &m_vecPlane[i], &mypos, &boxPos))
        {
            D3DXVECTOR3 p1out(pout - mypos);
            float p1outlen = D3DXVec3Length(&p1out);
            if (p1outlen < p12len)
            {
                results.emplace_back(static_cast<int>(i));
            }
        }
    }

    

    auto getV = [&m_vecWorld](int a, int b) { return m_vecWorld[b] - m_vecWorld[a]; };


    std::vector<D3DXVECTOR3> normals;
    for (auto& result : results)
    {
        D3DXVECTOR3 normal;
        switch (result)
        {
        case 0: D3DXVec3Cross(&normal, &getV(2, 3), &getV(3, 6)); break;
        case 1: D3DXVec3Cross(&normal, &getV(1, 0), &getV(0, 5)); break;
        case 2: D3DXVec3Cross(&normal, &getV(0, 2), &getV(2, 4)); break;
        case 3: D3DXVec3Cross(&normal, &getV(3, 1), &getV(1, 7)); break;
        case 4: D3DXVec3Cross(&normal, &getV(0, 1), &getV(1, 2)); break;
        case 5: D3DXVec3Cross(&normal, &getV(6, 7), &getV(7, 4)); break;
        }
        D3DXVec3Normalize(&normal, &normal);
        normals.emplace_back(normal);
    }

    return normals;
}

D3DXVECTOR3 Collision::GetCollidedNormal2(const D3DXVECTOR3& mypos, const BoundingBox& box)
{
    std::vector<D3DXVECTOR3> m_vecProj;
    m_vecProj.resize(8);
    m_vecProj[0] = (D3DXVECTOR3(-1, 1, 1));	    //ÁÂ»óÈÄ
    m_vecProj[1] = (D3DXVECTOR3(1, 1, 1));	    //¿ì»óÈÄ
    m_vecProj[2] = (D3DXVECTOR3(-1, 1, -1));	//ÁÂ»óÀü
    m_vecProj[3] = (D3DXVECTOR3(1, 1, -1));	    //¿ì»óÀü
    m_vecProj[4] = (D3DXVECTOR3(-1, -1, 1));	//ÁÂÇÏÈÄ
    m_vecProj[5] = (D3DXVECTOR3(1, -1, 1));	    //¿ìÇÏÈÄ
    m_vecProj[6] = (D3DXVECTOR3(-1, -1, -1));	//ÁÂÇÏÀü 
    m_vecProj[7] = (D3DXVECTOR3(1, -1, -1));	//¿ìÇÏÀü

    D3DXMATRIX s, r, t, m;
    D3DXMatrixScaling(&s, box.extent.x, box.extent.y, box.extent.z);
    D3DXMatrixRotationQuaternion(&r, &box.rotation);
    D3DXVECTOR3 boxPos(box.center + box.position);
    D3DXMatrixTranslation(&t, boxPos.x, boxPos.y, boxPos.z);
    m = s * r * t;

    std::vector<D3DXVECTOR3> m_vecWorld;
    m_vecWorld.resize(8);
    for (size_t i = 0; i < m_vecWorld.size(); ++i)
        D3DXVec3TransformCoord(&m_vecWorld[i], &m_vecProj[i], &m);

    std::vector<D3DXPLANE> m_vecPlane;
    m_vecPlane.resize(6);
    //±ÙÆò¸é//ÁÂ»óÀü//¿ì»óÀü//ÁÂÇÏÀü
    D3DXPlaneFromPoints(&m_vecPlane[0], &m_vecWorld[2], &m_vecWorld[3], &m_vecWorld[6]);
    //¿øÆò¸é//¿ì»óÈÄ//ÁÂ»óÈÄ//¿ìÇÏÈÄ
    D3DXPlaneFromPoints(&m_vecPlane[1], &m_vecWorld[1], &m_vecWorld[0], &m_vecWorld[5]);
    //ÁÂÆò¸é//ÁÂ»óÈÄ//ÁÂ»óÀü//ÁÂÇÏÈÄ
    D3DXPlaneFromPoints(&m_vecPlane[2], &m_vecWorld[0], &m_vecWorld[2], &m_vecWorld[4]);
    //¿ìÆò¸é//¿ì»óÀü//¿ì»óÈÄ//¿ìÇÏÀü
    D3DXPlaneFromPoints(&m_vecPlane[3], &m_vecWorld[3], &m_vecWorld[1], &m_vecWorld[7]);
    //»óÆò¸é//ÁÂ»óÈÄ//¿ì»óÈÄ//ÁÂ»óÀü
    D3DXPlaneFromPoints(&m_vecPlane[4], &m_vecWorld[0], &m_vecWorld[1], &m_vecWorld[2]);
    //ÇÏÆò¸é//ÁÂÇÏÀü//¿ìÇÏÀü//ÁÂÇÏÈÄ
    D3DXPlaneFromPoints(&m_vecPlane[5], &m_vecWorld[6], &m_vecWorld[7], &m_vecWorld[4]);

    std::vector<BoundingBox> sixPointsBox;
    sixPointsBox.resize(6);
    
    for (size_t i = 0; i < m_vecPlane.size(); ++i)
    {

        D3DXVECTOR3 pout;
        if (D3DXPlaneIntersectLine(&pout, &m_vecPlane[i], &mypos, &boxPos))
        {
            sixPointsBox[i].position = pout;
            sixPointsBox[i].extent = Vector3::ONE;
        }
    }

    D3DXVECTOR3 vdir = boxPos - mypos;
    float rayDistance = D3DXVec3Length(&vdir);
    D3DXVec3Normalize(&vdir, &vdir);

    Ray ray = Ray(mypos, vdir);
    int targetIndex = -1;
    for (size_t i = 0; i < 6; i++)
    {
        if (HasCollision(ray, sixPointsBox[i], rayDistance))
        {
            targetIndex = static_cast<int>(i);
            break;
        }
            
    }

    auto getV = [&m_vecWorld](int a, int b) { return m_vecWorld[b] - m_vecWorld[a]; };

    D3DXVECTOR3 normal;
    switch (targetIndex)
    {
    case 0: D3DXVec3Cross(&normal, &getV(2, 3), &getV(3, 6)); break;
    case 1: D3DXVec3Cross(&normal, &getV(1, 0), &getV(0, 5)); break;
    case 2: D3DXVec3Cross(&normal, &getV(0, 2), &getV(2, 4)); break;
    case 3: D3DXVec3Cross(&normal, &getV(3, 1), &getV(1, 7)); break;
    case 4: D3DXVec3Cross(&normal, &getV(0, 1), &getV(1, 2)); break;
    case 5: D3DXVec3Cross(&normal, &getV(6, 7), &getV(7, 4)); break;
    }
    D3DXVec3Normalize(&normal, &normal);

    return normal;
}

bool Collision::HasCollision(const BoundingBox& lhs, const BoundingBox& rhs)
{
    D3DXMATRIX A_transform, B_transform;
    D3DXMatrixRotationQuaternion(&A_transform, &lhs.rotation);
    D3DXMatrixRotationQuaternion(&B_transform, &rhs.rotation);

    const D3DXVECTOR3& A_extent = lhs.extent;
    const D3DXVECTOR3& B_extent = rhs.extent;

    const D3DXVECTOR3 distance = 
        (lhs.center + lhs.position) - (rhs.center + rhs.position);

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

bool Collision::HasCollision(
    const Ray& ray, 
    const BoundingBox& box, 
    float* OutDistance)
{
    float tMin = 0.0f;
    float tMax = ICamera::VISUAL_RANGE;

    const D3DXVECTOR3 diff = (box.center + box.position) - ray.m_pos;

    D3DXMATRIX m;
    D3DXMatrixRotationQuaternion(&m, &box.rotation);

    const D3DXVECTOR3 xAxis(m._11, m._12, m._13);
    float e = D3DXVec3Dot(&xAxis, &diff);
    float f = D3DXVec3Dot(&ray.m_dir, &xAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - box.extent.x) / f;
        float t2 = (e + box.extent.x) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - box.extent.x > 0.0f || -e + box.extent.x < 0.0f)
            return false;
    }

    const D3DXVECTOR3 yAxis(m._21, m._22, m._23);
    e = D3DXVec3Dot(&yAxis, &diff);
    f = D3DXVec3Dot(&ray.m_dir, &yAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - box.extent.y) / f;
        float t2 = (e + box.extent.y) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - box.extent.y > 0.0f || -e + box.extent.y < 0.0f)
            return false;
    }

    const D3DXVECTOR3 zAxis(m._31, m._32, m._33);
    e = D3DXVec3Dot(&zAxis, &diff);
    f = D3DXVec3Dot(&ray.m_dir, &zAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - box.extent.z) / f;
        float t2 = (e + box.extent.z) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - box.extent.z > 0.0f || -e + box.extent.z < 0.0f)
            return false;
    }

    *OutDistance = tMin;
    return true;
}

bool Collision::HasCollision(const Ray & ray, const BoundingBox & box, const float rayEnd)
{
    float tMin = 0.0f;
    float tMax = rayEnd;

    const D3DXVECTOR3 diff = (box.center + box.position) - ray.m_pos;

    D3DXMATRIX m;
    D3DXMatrixRotationQuaternion(&m, &box.rotation);

    const D3DXVECTOR3 xAxis(m._11, m._12, m._13);
    float e = D3DXVec3Dot(&xAxis, &diff);
    float f = D3DXVec3Dot(&ray.m_dir, &xAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - box.extent.x) / f;
        float t2 = (e + box.extent.x) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - box.extent.x > 0.0f || -e + box.extent.x < 0.0f)
            return false;
    }

    const D3DXVECTOR3 yAxis(m._21, m._22, m._23);
    e = D3DXVec3Dot(&yAxis, &diff);
    f = D3DXVec3Dot(&ray.m_dir, &yAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - box.extent.y) / f;
        float t2 = (e + box.extent.y) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - box.extent.y > 0.0f || -e + box.extent.y < 0.0f)
            return false;
    }

    const D3DXVECTOR3 zAxis(m._31, m._32, m._33);
    e = D3DXVec3Dot(&zAxis, &diff);
    f = D3DXVec3Dot(&ray.m_dir, &zAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - box.extent.z) / f;
        float t2 = (e + box.extent.z) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - box.extent.z > 0.0f || -e + box.extent.z < 0.0f)
            return false;
    }

    return true;
}

bool Collision::HasCollision(const Ray& ray, const BoundingRect& rect)
{
    float tMin = 0.0f;
    float tMax = ICamera::VISUAL_RANGE;

    const D3DXVECTOR2 rayPos(ray.m_pos.x, ray.m_pos.z);
    D3DXVECTOR2 rayDir(ray.m_dir.x, ray.m_dir.z);
    D3DXVec2Normalize(&rayDir, &rayDir);

    const D3DXVECTOR2 diff 
        = (rect.center + rect.position) 
        - rayPos;

    const D3DXVECTOR2 xAxis(1.0f, 0.0f);
    float e = D3DXVec2Dot(&xAxis, &diff);
    float f = D3DXVec2Dot(&rayDir, &xAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - rect.extent.x) / f;
        float t2 = (e + rect.extent.x) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - rect.extent.x > 0.0f || -e + rect.extent.x < 0.0f)
            return false;
    }

    const D3DXVECTOR2 zAxis(0.0f, 1.0f);
    e = D3DXVec2Dot(&zAxis, &diff);
    f = D3DXVec2Dot(&rayDir, &zAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - rect.extent.y) / f;
        float t2 = (e + rect.extent.y) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - rect.extent.y > 0.0f || -e + rect.extent.y < 0.0f)
            return false;
    }

    return true;
}

bool Collision::HasCollision(const Ray & ray, const BoundingRect & rect, const float end)
{
    float tMin = 0.0f;
    float tMax = end;
    //D3DXVECTOR3 endlength = end - ray.m_pos;
    //float tMax = D3DXVec3Length(&endlength);
    
    const D3DXVECTOR2 rayPos(ray.m_pos.x, ray.m_pos.z);
    D3DXVECTOR2 rayDir(ray.m_dir.x, ray.m_dir.z);
    D3DXVec2Normalize(&rayDir, &rayDir);

    const D3DXVECTOR2 diff
        = (rect.center + rect.position)
        - rayPos;

    const D3DXVECTOR2 xAxis(1.0f, 0.0f);
    float e = D3DXVec2Dot(&xAxis, &diff);
    float f = D3DXVec2Dot(&rayDir, &xAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - rect.extent.x) / f;
        float t2 = (e + rect.extent.x) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - rect.extent.x > 0.0f || -e + rect.extent.x < 0.0f)
            return false;
    }

    const D3DXVECTOR2 zAxis(0.0f, 1.0f);
    e = D3DXVec2Dot(&zAxis, &diff);
    f = D3DXVec2Dot(&rayDir, &zAxis);

    if (std::abs(f) > D3DX_16F_EPSILON)
    {
        float t1 = (e - rect.extent.y) / f;
        float t2 = (e + rect.extent.y) / f;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t2 < tMax)
            tMax = t2;

        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else
    {
        if (-e - rect.extent.y > 0.0f || -e + rect.extent.y < 0.0f)
            return false;
    }

    return true;
}




BoundingRect::BoundingRect()
    : center(0.0f, 0.0f)
    , position(0.0f, 0.0f)
    , extent(0.0f, 0.0f)
{
}

BoundingRect::~BoundingRect()
{
}
