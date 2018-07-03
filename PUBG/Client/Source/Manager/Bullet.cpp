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

    auto pos = pTr->GetPosition();
    pos += m_dir * m_Speed;
    pTr->SetPosition(pos);
    pTr->Update();
    pBoxCollider->Update(pTr->GetTransformationMatrix());

}

void Bullet::OnRender()
{
    if (!m_IsActive) return;

    //// visualize cylider
    D3DXMATRIX mat = pTr->GetTransformationMatrix();

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        pCylinder,
        0,
        [this, &mat](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &mat);

        D3DXCOLOR red(1.0f, 0.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &red, sizeof red);
    });

}

void Bullet::Reset()
{
    pTr->SetPosition(Vector3::ZERO);
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

void Bullet::Set(const D3DXVECTOR3 & startPos, const D3DXVECTOR3 & dir, 
    const float speed, const float damage, const TAG_COLLISION tag)
{
    pTr = GetTransform();
    pTr->SetPosition(startPos);
    pTr->Update();
    m_dir = dir;
    m_Speed = speed;
    m_Damage = damage;
    pBoxCollider->SetTag(tag);
    m_IsActive = true;

    CurrentScene()()->AddObject(this);
}


_BulletPool::_BulletPool()
    : Singleton<_BulletPool>()
{
    D3DXCreateCylinder(Device()(), 3.0f, 3.0f, 3.0f, 10, 10, &m_pCylinder, 
        nullptr);
}

_BulletPool::~_BulletPool()
{
}

void _BulletPool::Destroy()
{
    //for (auto b : m_Bullets)
    //    SAFE_DELETE(b);

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


Bullet * _BulletPool::Fire(const D3DXVECTOR3 & startPos,
    const D3DXVECTOR3 & dir, const float speed, const float damage,
    const TAG_COLLISION tag)
{
    for (auto& b : m_Bullets)
    {
        if (!b->IsActive())
        {
            b->Set(startPos, dir, speed, damage, tag);
            return b;
        }
    }

    Bullet* b = new Bullet;
    m_Bullets.emplace_back(b);
    b->Set(startPos, dir, speed, damage, tag);
    return b;
}

LPD3DXMESH _BulletPool::GetCylinder() const
{
    return m_pCylinder;
}
