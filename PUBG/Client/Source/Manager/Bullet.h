#pragma once
#include "IObject.h"
#include "Singleton.h"
//#include "Protocol.h"

class BoxCollider;

class Bullet : public IObject
{
    struct HitTargetInfo {
        D3DXVECTOR3 pos;
        TAG_RES_STATIC tag_Weapon;
        TAG_COLLIDER_CHARACTER_PART tag_chrPart;
        Character* chr;

        HitTargetInfo();
        HitTargetInfo(D3DXVECTOR3 _pos, 
            TAG_RES_STATIC _tag_Weapon, 
            TAG_COLLIDER_CHARACTER_PART _tag_chrPart,
            Character* obj
        );
    };
private:
    IScene*                 pCurrentScene;
    GameInfo::MyInfo        m_myInfo;
    TAG_RES_STATIC          m_tag;

    D3DXVECTOR3             m_curPos;
    D3DXVECTOR3             m_nextPos;

    Transform*              pTr;
    D3DXVECTOR3             m_dir;
    float                   m_Speed;
    float                   m_Damage;
    bool                    m_IsActive;
    
    LPD3DXMESH              pCylinder;

public:
    Bullet();
    virtual ~Bullet();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void Set(GameInfo::MyInfo m_myInfo, const D3DXVECTOR3& startPos, const D3DXVECTOR3& dir,
        const float speed, const float damage, TAG_RES_STATIC tag);
    void Reset();

    bool IsActive() const;
    float GetSpeed() const;
    float GetDamage() const;
};

class _BulletPool : public Singleton<_BulletPool>
{
private:
    deque<Bullet*> m_Bullets;
    LPD3DXMESH    m_pCylinder;
    BoundingSphere m_targetHitSphere;

    _BulletPool();
    ~_BulletPool();

public:
    void Destroy();
    void PrintNumBullet();
    void Render();

    //Bullet* Fire(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation,
    //    const float speed, const float damage, const TAG_COLLISION tag);

    Bullet* Fire(GameInfo::MyInfo myInfo,
        const D3DXVECTOR3& startPos, 
        const D3DXVECTOR3& dir,
        const float speed, 
        const float damage, TAG_RES_STATIC tag);

    LPD3DXMESH GetCylinder() const;

    void SetTargetHitSphere(const D3DXVECTOR3& pos);

    friend Singleton<_BulletPool>;
};

struct BulletPool
{
    _BulletPool* operator()()
    {
        return _BulletPool::GetInstance();
    }
};