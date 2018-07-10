#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "CharacterPart.h"
#include "DirectionalLight.h"
#include "AnimationState.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
#include "HeightMap.h"
#include "ScenePlay.h"
#include "Ballistics.h"
#include "TerrainFeature.h"

void Character::RifleShootingTest() //bullet 객체에 대한
{
    //bullet cool down 시간
    auto& inven = m_totalInventory;
    inven.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(inven.m_pHand->GetTagResStatic());
    //bullet 갯수 만큼 조정
    int numBullet = inven.m_pHand->GetNumBullet();
    inven.m_pHand->SetNumBullet(--numBullet);
    cout << "총에 남아있는 총알 개수: " << inven.m_pHand->GetNumBullet() << "\n";
    //bullet이 나가는 포지션 구하기
    D3DXMATRIX mat
        = inven.m_pHand->GetGunBolt()->CombinedTransformationMatrix  //model space combinde matrix
        * m_framePtr.pHandGun->CombinedTransformationMatrix // hand gun space matrix
        * GetTransform()->GetTransformationMatrix();    //character world matrix
    D3DXVECTOR3 bulletFirePos = Matrix::GetTranslation(mat);

    //bullet의 direction 구하기
    D3DXVECTOR3 bulletDir;
    CurrentCamera()()->CalcPickedPosition(&bulletDir, 1280 / 2, 720 / 2);
    bulletDir = bulletDir - bulletFirePos;
    D3DXVec3Normalize(&bulletDir, &bulletDir);

    // =====총알 객체가 나가는 부분 ===========
    switch (inven.m_pHand->GetTagResStatic())
    {
    case TAG_RES_STATIC::QBZ:
        BulletPool()()->Fire(Communication()()->m_myInfo, bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::QBZ), ItemInfo::GetBaseDamage(TAG_RES_STATIC::QBZ), TAG_COLLISION::Impassable);
        break;
    case TAG_RES_STATIC::Kar98k:
    {
        BulletPool()()->Fire(Communication()()->m_myInfo, bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::Kar98k), ItemInfo::GetBaseDamage(TAG_RES_STATIC::Kar98k), TAG_COLLISION::Impassable);

        //Kar98k BoltAction Animation
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Base;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Prone;

        assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::RifleShooting(), tagAnim is COUNT");

        m_hasChangingState = true;

        //총 자체 애니메이션
        m_isNeedRifleAnim = true;
        inven.m_pHand->Set
        (
            TAG_ANIM_WEAPON::Weapon_Kar98k_BoltAction_1,
            false,
            Item::DEFAULT_BLENDING_TIME,
            Item::DEFAULT_NEXT_WEIGHT,
            Item::DEFAULT_POSITION,
            Item::DEFAULT_FINISH_EVENT_AGO_TIME,
            [this, &inven]() {
            inven.m_pHand->Set(
                TAG_ANIM_WEAPON::Weapon_Kar98k_Idle,
                false);
            m_isNeedRifleAnim = false;
        });

        //캐릭터의 애니메이션
        setAnimation(
            CharacterAnimation::BodyPart::UPPER,
            tagAnim,
            true, //ok
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f, //ok
            [this]()
        {
            m_hasChangingState = false;
            setAnimation(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                true,
                0.3f);
        });
    }
    break;
    }

}

D3DXVECTOR3 Character::FindShootingTargetPos()
{
    ////이 함수는 가장 가까운 Object를 찾아서 

    ////Screen에서 쏜 Ray 방향으로 cell들이 담긴다.
    //IScene* CS = CurrentScene()();
    //Ray ray = Ray::RayAtWorldSpace(1280 / 2, 720 / 2);
    //CS->m_RayArea.CreateRayArea(&ray);

    //vector<D3DXVECTOR3> vecP; //이곳에서 가장 가까운 pos를 구할 것이다

    ////1.terrain과의 충돌
    //auto tfs(CS->m_RayArea.GetTerrainFeatures());
    //for (auto tf : tfs)
    //{
    //    // 먼저 terrain features의 바운딩스피어와 충돌을 검사한다.
    //    BoundingSphere bs = tf->GetBoundingSphere();

    //    if (!D3DXSphereBoundProbe(
    //        &(bs.center + bs.position),
    //        bs.radius,
    //        &ray.m_pos,
    //        &ray.m_dir)) continue;


    //    // 물체의 바운딩박스들과 충돌을 검사한다.
    //    float minDist = std::numeric_limits<float>::max();
    //    float dist = std::numeric_limits<float>::max();

    //    const auto& obbs = tf->GetBoundingBoxes();
    //    for (std::size_t i = 0; i < obbs.size(); i++)
    //    {
    //        auto& obb = obbs[i];

    //        if (Collision::HasCollision(ray, obb, &dist))
    //        {
    //            // hit
    //            if (dist < minDist)
    //            {
    //                minDist = dist;

    //            }
    //        }
    //    }
    //    if (minDist != std::numeric_limits<float>::max())
    //    {
    //        vecP.emplace_back(ray.m_pos + ray.m_dir * minDist);
    //    }
    //}

    //////2. character와의 충돌
    ////auto chrs(CS->m_RayArea.GetCharacters());
    ////for (auto chr : chrs)
    ////{
    ////    // 먼저 캐릭터의 바운딩스피어와 충돌을 검사한다.
    ////    BoundingSphere bs = chr->GetBoundingSphere();

    ////    if (!D3DXSphereBoundProbe(
    ////        &(bs.center + bs.position),
    ////        bs.radius,
    ////        &ray.m_pos,
    ////        &ray.m_dir)) continue;

    ////    

    ////}
    //D3DXVECTOR3 targetPos;
    //float save = FLT_MAX;
    //for (int i = 0; i < vecP.size(); i++)
    //{
    //    float length = D3DXVec3Length(&(vecP[i] - ray.m_pos));
    //    if (save > length)
    //    {
    //        save = length;
    //        targetPos = vecP[i];
    //    }
    //}

    //return targetPos;
    return D3DXVECTOR3(0, 0, 0);
}
