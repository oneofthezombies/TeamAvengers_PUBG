#include "stdafx.h"
#include "Bullet.h"
#include "Collider.h"

Bullet::Bullet()
    : IObject()

    , m_Speed(0.0f)
    , m_Damage(0.0f)
    , m_IsActive(false)
    
    , pBoxCollider(nullptr)
{
    GetTransform()->SetPosition(Vector3::ZERO);

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->Init(-Vector3::ONE, Vector3::ONE);
    pBoxCollider->SetTag(TAG_COLLISION::Idle);

    pCylinder = BulletPool()()->GetCylinder();
}

Bullet::~Bullet()
{
}

void Bullet::OnUpdate()
{
    if (!m_IsActive) return;

    auto tr = GetTransform();

    auto pos = tr->GetPosition();
    D3DXMATRIX r;
    D3DXMatrixRotationQuaternion(&r, &tr->GetRotation());
    D3DXVECTOR3 forward;
    D3DXVec3TransformNormal(&forward, &Vector3::FORWARD, &r);
    D3DXVec3Normalize(&forward, &forward);
    pos += forward * m_Speed;
    tr->SetPosition(pos);

    pBoxCollider->Update(tr->GetTransformationMatrix());
}

void Bullet::OnRender()
{
    if (!m_IsActive) return;

    auto pDevice = Device()();
    pDevice->SetRenderState(D3DRS_LIGHTING, true);
    pDevice->SetTexture(0, nullptr);
    pDevice->SetMaterial(&MaterialTemplate::GetWhite());
    pDevice->SetTransform(
        D3DTS_WORLD, &GetTransform()->GetTransformationMatrix());
    pCylinder->DrawSubset(0);
}

void Bullet::Reset()
{
    GetTransform()->SetPosition(Vector3::ZERO);
    pBoxCollider->SetTag(TAG_COLLISION::Idle);
    m_IsActive = false;

    CurrentScene()()->RemoveObject(this);
}

bool Bullet::IsActive() const
{
    return m_IsActive;
}

float Bullet::GetSpeed() const
{
    return m_Speed;
}

float Bullet::GetDamage() const
{
    return m_Damage;
}

TAG_COLLISION Bullet::GetTagCollision() const
{
    return pBoxCollider->GetTag();
}

void Bullet::Set(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, 
    const float speed, const float damage, const TAG_COLLISION tag)
{
    auto tr = GetTransform();
    tr->SetPosition(position);
    tr->SetRotation(rotation);
    m_Speed = speed;
    m_Damage = damage;
    pBoxCollider->SetTag(tag);
    m_IsActive = true;

    CurrentScene()()->AddObject(this);
}

_BulletPool::_BulletPool()
    : Singleton<_BulletPool>()
{
    D3DXCreateCylinder(Device()(), 1.0f, 1.0f, 1.0f, 10, 10, &m_pCylinder, 
        nullptr);
}

_BulletPool::~_BulletPool()
{
}

void _BulletPool::Destroy()
{
    for (auto b : m_Bullets)
        SAFE_DELETE(b);

    SAFE_RELEASE(m_pCylinder);
}

void _BulletPool::PrintNumBullet()
{
    Debug << "Total number of bullets : " << m_Bullets.size() << '\n';

    int count = 0;
    for (auto& b : m_Bullets)
    {
        if (b->IsActive())
            ++count;
    }
    Debug << "Current number of active bullets : " << count << '\n';
}

Bullet* _BulletPool::Fire(const D3DXVECTOR3& position,
    const D3DXQUATERNION& rotation, const float speed, const float damage,
    const TAG_COLLISION tag)
{
    for (auto& b : m_Bullets)
    {
        if (!b->IsActive())
        {
            b->Set(position, rotation, speed, damage, tag);
            return b;
        }
    }

    Bullet* b = new Bullet;
    m_Bullets.emplace_back(b);
    b->Set(position, rotation, speed, damage, tag);
    return b;
}

LPD3DXMESH _BulletPool::GetCylinder() const
{
    return m_pCylinder;
}
