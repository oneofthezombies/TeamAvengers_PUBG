#include "stdafx.h"
#include "Bullet.h"
#include "Collider.h"
#include "TerrainFeature.h"
#include "Character.h"
#include "ItemInfo.h"

Bullet::Bullet()
    : IObject(TAG_OBJECT::Bullet)
    , m_Speed(0.0f)
    , m_Damage(0.0f)
    , m_IsActive(false)
    , pCylinder(nullptr)
    //, pBoxCollider(nullptr)
    , pCurrentScene(nullptr)
{
    GetTransform()->SetPosition(Vector3::ZERO);

    //pBoxCollider = AddComponent<BoxCollider>();
    //pBoxCollider->Init(-Vector3::ONE, Vector3::ONE);
    //pBoxCollider->SetTagCollision(TAG_COLLISION::Idle);

    pCylinder = BulletPool()()->GetCylinder();
    pCurrentScene = CurrentScene()();
    if (!pCurrentScene)
        assert(false && "Bullet() no current Scene");
}

Bullet::~Bullet()
{
}
Bullet::HitTargetInfo::HitTargetInfo()
    : pos(Vector3::ZERO)
    , tag_Weapon(TAG_RES_STATIC::COUNT)
    , tag_chrPart(TAG_COLLIDER_CHARACTER_PART::COUNT)
    , chr(nullptr)
{
}

Bullet::HitTargetInfo::HitTargetInfo(D3DXVECTOR3 _pos, TAG_RES_STATIC _tag_Weapon, TAG_COLLIDER_CHARACTER_PART _tag_chrPart, Character* obj)
    : pos(_pos)
    , tag_Weapon(_tag_Weapon)
    , tag_chrPart(_tag_chrPart)
    , chr(obj)
{
}


void Bullet::OnUpdate()
{
    if (!m_IsActive) return;
    
     m_nextPos = m_curPos = pTr->GetPosition();
     //JHTODO
     m_nextPos += m_dir * m_Speed
         /**ItemInfo::GetDropOffByDistance(1.0f,GetTagObject())*/ 
         * Time()()->GetDeltaTime();
     
     Ray ray;
     ray.m_pos = m_curPos;
     ray.m_dir = m_nextPos - m_curPos;
     float rayLength = D3DXVec3Length(&ray.m_dir);
     D3DXVec3Normalize(&ray.m_dir,&ray.m_dir);
     

     //Screen���� �� Ray �������� cell���� ����.
     IScene* CS = CurrentScene()();
     CS->m_RayArea.CreateRayArea(&ray, rayLength);


     vector<HitTargetInfo> vecHitTargetInfo;

     //1. Terrain features���� �浹
     auto tfs(CS->m_RayArea.GetTerrainFeatures());
     for (auto tf : tfs)
     {
         // ���� terrain features�� �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
         BoundingSphere bs = tf->GetBoundingSphere();

         if (!D3DXSphereBoundProbe(
             &(bs.center + bs.position),
             bs.radius,
             &ray.m_pos,
             &ray.m_dir)) continue;


         // ��ü�� �ٿ���ڽ���� �浹�� �˻��Ѵ�.
         float minDist = std::numeric_limits<float>::max();
         float dist = std::numeric_limits<float>::max();

         const auto& obbs = tf->GetBoundingBoxes();
         for (std::size_t i = 0; i < obbs.size(); i++)
         {
             auto& obb = obbs[i];

             if (Collision::HasCollision(ray, obb, &dist))
             {
                 // hit
                 if (dist < minDist)
                 {
                     minDist = dist;
                 }
             }
         }
         if (minDist != std::numeric_limits<float>::max())
         {
             //vecTargetPos.emplace_back(ray.m_pos + ray.m_dir * minDist);//���� target���� ã�Ƴ���
             vecHitTargetInfo.emplace_back(HitTargetInfo((ray.m_pos + ray.m_dir * minDist), m_tag, TAG_COLLIDER_CHARACTER_PART::COUNT,nullptr));
         }
     }


     //2. character���� �浹
     auto chrs(CS->m_RayArea.GetCharacters());
     for (auto chr : chrs)
     {
         if (m_myInfo.ID == chr->GetIndex()) continue;

         // ���� ĳ������ �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
         BoundingSphere bs = chr->GetBoundingSphere();

         if (!D3DXSphereBoundProbe(
             &(bs.center + bs.position),
             bs.radius,
             &ray.m_pos,
             &ray.m_dir)) continue;

         // ĳ���Ϳ� �ٿ���ڽ���� �浹�� �˻��Ѵ�.
         float minDist = std::numeric_limits<float>::max();
         float dist = std::numeric_limits<float>::max();

         const auto& obbs = chr->GetBoundingBoxes();
         size_t otherHitPart = -1;
         for (std::size_t i = 0; i < obbs.size(); i++)
         {
             auto& obb = obbs[i];

             if (Collision::HasCollision(ray, obb, &dist))
             {
                 // hit
                 if (dist < minDist)
                 {
                     minDist = dist;
                     otherHitPart = i;
                 }
             }
         }
         if (minDist != std::numeric_limits<float>::max())
         {
             //vecTargetPos.emplace_back(ray.m_pos + ray.m_dir * minDist);//���� target���� ã�Ƴ���
             vecHitTargetInfo.emplace_back(HitTargetInfo((ray.m_pos + ray.m_dir * minDist), m_tag, static_cast<TAG_COLLIDER_CHARACTER_PART>(otherHitPart), chr));
         }
     }

     float shortestLength = FLT_MAX;
     HitTargetInfo targetInfo;
     for (size_t i = 0; i < vecHitTargetInfo.size(); i++)//���� target�� �� ���� distance�� ª�� ���� ã�Ƴ���.
     {
         if (shortestLength > D3DXVec3Length(&vecHitTargetInfo[i].pos))
         {
             shortestLength = D3DXVec3Length(&vecHitTargetInfo[i].pos);
             targetInfo = vecHitTargetInfo[i];
         }
     }

     //�Ѱ��̶� ���� �κ��� �ִٸ�      
     if (shortestLength != FLT_MAX)
     {

         BulletPool()()->SetTargetHitSphere(targetInfo.pos);//<<�̰��� ���� �κ�, ȭ�鿡 ���� ������ render�ϱ� ���� ������ �κ�
         //Communication()()-> ��Ʈ��ũ�� �� �������� �༭ ��� ����� �̰��� �¾Ҵٴ� ���� ǥ�� �Ǵ� ���� ���̵��� ����
         

         
         if (targetInfo.tag_chrPart != TAG_COLLIDER_CHARACTER_PART::COUNT) //�ɸ��Ϳ� ���� ���̶��
         {
             //������� �´´ٸ� �ǰ� ������
             const float damage
                 = ItemInfo::GetBaseDamage(targetInfo.tag_Weapon)//Base Weapon Damage
                 * ItemInfo::GetDropOffByDistance(shortestLength, targetInfo.tag_Weapon)//Damage drop-off by Distance
                 * CharacterInfo::GetHitAreaDamage(targetInfo.tag_chrPart) //Hit Area Damage
                 * CharacterInfo::GetWeaponClassDamageByHitZone(targetInfo.tag_chrPart); //Weapon Class Damage By Hit Zone

             targetInfo.chr->MinusDamage(damage);
             Communication()()->SendEventMinusDamage(targetInfo.chr->GetIndex(), damage);
         }
         else
         {
             //terrain features �� ���� ���̶�� 
             //ź��         //���� �´´ٸ� ���� ź���� ������
         }

         m_IsActive = false;
         return;
     }
     
     //deactivating bullet when it is out of boundary
     if (pCurrentScene->isOutOfBoundaryBox(m_curPos))
     {
         m_IsActive = false;
         return;
     }
         


     //���� ���� ���ٸ� ��� ������Ʈ
     m_curPos = m_nextPos;
     pTr->SetPosition(m_curPos);
     pTr->Update();

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
    //pBoxCollider->SetTagCollision(TAG_COLLISION::Idle);
    m_IsActive = false;

    pCurrentScene->RemoveObject(this);
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

//TAG_COLLISION Bullet::GetTagCollision() const
//{
//    return pBoxCollider->GetTagCollision();
//}

void Bullet::Set(GameInfo::MyInfo myInfo, const D3DXVECTOR3 & startPos, const D3DXVECTOR3 & dir,
    const float speed, const float damage, TAG_RES_STATIC tag)
{
    m_myInfo = myInfo;
    m_tag = tag;

    m_curPos = startPos;

    pTr = GetTransform();
    pTr->SetPosition(startPos);
    pTr->Update();
    m_dir = dir;
    m_Speed = speed;
    m_Damage = damage;
    m_IsActive = true;

    pCurrentScene->AddObject(this);

}


_BulletPool::_BulletPool()
    : Singleton<_BulletPool>()
{
    D3DXCreateCylinder(Device()(), 3.0f, 3.0f, 3.0f, 10, 10, &m_pCylinder, 
        nullptr);


    m_targetHitSphere.radius = 10.0f;
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

void _BulletPool::Render()
{
//    if (!Collision()()->IsRender()) return;

    //hit �� position�� ��ü �׸���
    D3DXMATRIX m;
    D3DXMatrixTransformation(
        &m,
        nullptr,
        nullptr,
        &D3DXVECTOR3(m_targetHitSphere.radius, m_targetHitSphere.radius, m_targetHitSphere.radius),
        nullptr, nullptr,
        &(m_targetHitSphere.center + m_targetHitSphere.position));

    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        Resource()()->GetBoundingSphereMesh(),
        0,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR Red(1.0f, 0.f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &Red, sizeof Red);
    });

}

Bullet* _BulletPool::Fire(
    GameInfo::MyInfo m_myInfo, 
    const D3DXVECTOR3& startPos,
    const D3DXVECTOR3& dir, 
    const float speed, 
    const float damage,
    TAG_RES_STATIC tag)
{
    for (auto& b : m_Bullets)
    {
        if (!b->IsActive())
        {
            b->Set(m_myInfo, startPos, dir, speed, damage, tag);
            return b;
        }
    }

    Bullet* b = new Bullet;
    m_Bullets.emplace_back(b);
    b->Set(m_myInfo, startPos, dir, speed, damage, tag);
    return b;
}

LPD3DXMESH _BulletPool::GetCylinder() const
{
    return m_pCylinder;
}

void _BulletPool::SetTargetHitSphere(const D3DXVECTOR3& pos)
{
    m_targetHitSphere.position = pos;
}

