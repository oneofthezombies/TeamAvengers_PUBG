#include "stdafx.h"
#include "Bullet.h"
#include "Collider.h"
#include "TerrainFeature.h"


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

void Bullet::OnUpdate()
{
    if (!m_IsActive) return;


     m_nextPos = m_curPos = pTr->GetPosition();
     m_nextPos += m_dir * m_Speed;
     
     Ray ray;
     ray.m_pos = m_curPos;
     ray.m_dir = m_nextPos - m_curPos;
     float rayLength = D3DXVec3Length(&ray.m_dir);
     D3DXVec3Normalize(&ray.m_dir,&ray.m_dir);
     

     //Screen에서 쏜 Ray 방향으로 cell들이 담긴다.
     IScene* CS = CurrentScene()();
     CS->m_RayArea.CreateRayArea(&ray, rayLength);


     vector<D3DXVECTOR3> vecTargetPos;

     //1. Terrain features와의 충돌
     auto tfs(CS->m_RayArea.GetTerrainFeatures());
     for (auto tf : tfs)
     {
         // 먼저 terrain features의 바운딩스피어와 충돌을 검사한다.
         BoundingSphere bs = tf->GetBoundingSphere();

         if (!D3DXSphereBoundProbe(
             &(bs.center + bs.position),
             bs.radius,
             &ray.m_pos,
             &ray.m_dir)) continue;


         // 물체의 바운딩박스들과 충돌을 검사한다.
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
             vecTargetPos.emplace_back(ray.m_pos + ray.m_dir * minDist);
         }
     }


     //2. character와의 충돌
     auto chrs(CS->m_RayArea.GetCharacters());
     for (auto chr : chrs)
     {
         // 먼저 캐릭터의 바운딩스피어와 충돌을 검사한다.
         BoundingSphere bs = chr->GetBoundingSphere();

         if (!D3DXSphereBoundProbe(
             &(bs.center + bs.position),
             bs.radius,
             &ray.m_pos,
             &ray.m_dir)) continue;

         // 캐릭터와 바운딩박스들과 충돌을 검사한다.
         float minDist = std::numeric_limits<float>::max();
         float dist = std::numeric_limits<float>::max();

         const auto& obbs = chr->GetBoundingBoxes();
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
             vecTargetPos.emplace_back(ray.m_pos + ray.m_dir * minDist);
         }
     }

     float shortestLength = FLT_MAX;
     D3DXVECTOR3 targetPos;
     for (int i = 0; i < vecTargetPos.size(); i++)
     {
         if (shortestLength > D3DXVec3Length(&vecTargetPos[i]))
         {
             shortestLength = D3DXVec3Length(&vecTargetPos[i]);
             targetPos = vecTargetPos[i];
         }
     }

     targetPos; //<<이곳이 바로 가장 까까운 맞은 부분
     if (shortestLength != FLT_MAX)
     {

         m_IsActive = false;
         return;
     }
     
     //deactivating bullet when it is out of boundary
     if (pCurrentScene->isOutOfBoundaryBox(m_curPos))
     {
         m_IsActive = false;
         return;
     }
         


     //맞은 총이 없다면 계속 업데이트
     m_curPos = m_nextPos;
     pTr->SetPosition(m_curPos);
     pTr->Update();

     ////change cell space while moving
     //size_t nextCellSpace = pCurrentScene->GetCellIndex(m_curPos);
     //if (m_CellSpaceIndex != nextCellSpace&&m_IsActive)
     //{
     //    pCurrentScene->MoveCell(&m_CellSpaceIndex, nextCellSpace, TAG_OBJECT::Bullet, this);
     //}
    //auto pos = pTr->GetPosition();
    //pos += m_dir * m_Speed;
    //pTr->SetPosition(pos);
    //pTr->Update();
    ////pBoxCollider->Update(pTr->GetTransformationMatrix());

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
    pBoxCollider->SetTagCollision(TAG_COLLISION::Idle);
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

bool Bullet::CheckCollision()
{



    return false;
}

TAG_COLLISION Bullet::GetTagCollision() const
{
    return pBoxCollider->GetTagCollision();
}

void Bullet::Set(GameInfo::MyInfo myInfo, const D3DXVECTOR3 & startPos, const D3DXVECTOR3 & dir,
    const float speed, const float damage, TAG_COLLISION tag)
{
    m_myInfo = myInfo;

    m_curPos = startPos;

    pTr = GetTransform();
    pTr->SetPosition(startPos);
    pTr->Update();
    m_dir = dir;
    m_Speed = speed;
    m_Damage = damage;
    //pBoxCollider->SetTagCollision(tag);
    m_IsActive = true;

    pCurrentScene->AddObject(this);

    //m_CellSpaceIndex = pCurrentScene->GetCellIndex(pTr->GetPosition());
    //pCurrentScene->InsertObjIntoTotalCellSpace(TAG_OBJECT::Bullet, m_CellSpaceIndex, this);
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

Bullet* _BulletPool::Fire(
    GameInfo::MyInfo m_myInfo, 
    const D3DXVECTOR3& startPos,
    const D3DXVECTOR3& dir, 
    const float speed, 
    const float damage,
    TAG_COLLISION tag)
{
    for (auto& b : m_Bullets)
    {
        if (!b->IsActive())
        {
            b->Set(m_myInfo, startPos, dir, speed, damage,tag);
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
