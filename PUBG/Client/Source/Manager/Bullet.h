#pragma once
#include "IObject.h"
#include "Singleton.h"
#include "Protocol.h"

class BoxCollider;

class Bullet : public IObject
{
private:
    D3DXVECTOR3 m_dir;
    float       m_Speed;
    float       m_Damage;
    bool        m_IsActive;
    
    Transform*  pTr;

    LPD3DXMESH   pCylinder;
    BoxCollider* pBoxCollider;

public:
    Bullet();
    virtual ~Bullet();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    //void Set(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation,
    //    const float speed, const float damage, const TAG_COLLISION tag);
    void Set(const D3DXVECTOR3& startPos, const D3DXVECTOR3& dir,
        const float speed, const float damage, const TAG_COLLISION tag);
    void Reset();

    bool IsActive() const;
    float GetSpeed() const;
    float GetDamage() const;
    TAG_COLLISION GetTagCollision() const;
};

class _BulletPool : public Singleton<_BulletPool>
{
private:
    deque<Bullet*> m_Bullets;
    LPD3DXMESH    m_pCylinder;

    _BulletPool();
    ~_BulletPool();

public:
    void Destroy();
    void PrintNumBullet();

    //Bullet* Fire(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation,
    //    const float speed, const float damage, const TAG_COLLISION tag);

    Bullet* Fire(const D3DXVECTOR3& startPos, const D3DXVECTOR3& dir,
        const float speed, const float damage, const TAG_COLLISION tag);

    LPD3DXMESH GetCylinder() const;

    friend Singleton<_BulletPool>;
};

struct BulletPool
{
    _BulletPool* operator()()
    {
        return _BulletPool::GetInstance();
    }
};