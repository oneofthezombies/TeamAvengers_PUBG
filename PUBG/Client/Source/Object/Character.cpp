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

void Character::RifleShootingTest() //bullet ��ü�� ����
{
    //bullet cool down �ð�
    auto& inven = m_totalInventory;
    inven.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(inven.m_pHand->GetTagResStatic());
    //bullet ���� ��ŭ ����
    int numBullet = inven.m_pHand->GetNumBullet();
    inven.m_pHand->SetNumBullet(--numBullet);
    cout << "�ѿ� �����ִ� �Ѿ� ����: " << inven.m_pHand->GetNumBullet() << "\n";
    //bullet�� ������ ������ ���ϱ�
    D3DXMATRIX mat
        = inven.m_pHand->GetGunBolt()->CombinedTransformationMatrix  //model space combinde matrix
        * m_framePtr.pHandGun->CombinedTransformationMatrix // hand gun space matrix
        * GetTransform()->GetTransformationMatrix();    //character world matrix
    D3DXVECTOR3 bulletFirePos = Matrix::GetTranslation(mat);

    //bullet�� direction ���ϱ�
    D3DXVECTOR3 bulletDir;
    CurrentCamera()()->CalcPickedPosition(&bulletDir, 1280 / 2, 720 / 2);
    bulletDir = bulletDir - bulletFirePos;
    D3DXVec3Normalize(&bulletDir, &bulletDir);

    // =====�Ѿ� ��ü�� ������ �κ� ===========
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

        //�� ��ü �ִϸ��̼�
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

        //ĳ������ �ִϸ��̼�
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
    ////�� �Լ��� ���� ����� Object�� ã�Ƽ� 

    ////Screen���� �� Ray �������� cell���� ����.
    //IScene* CS = CurrentScene()();
    //Ray ray = Ray::RayAtWorldSpace(1280 / 2, 720 / 2);
    //CS->m_RayArea.CreateRayArea(&ray);

    //vector<D3DXVECTOR3> vecP; //�̰����� ���� ����� pos�� ���� ���̴�

    ////1.terrain���� �浹
    //auto tfs(CS->m_RayArea.GetTerrainFeatures());
    //for (auto tf : tfs)
    //{
    //    // ���� terrain features�� �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
    //    BoundingSphere bs = tf->GetBoundingSphere();

    //    if (!D3DXSphereBoundProbe(
    //        &(bs.center + bs.position),
    //        bs.radius,
    //        &ray.m_pos,
    //        &ray.m_dir)) continue;


    //    // ��ü�� �ٿ���ڽ���� �浹�� �˻��Ѵ�.
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

    //////2. character���� �浹
    ////auto chrs(CS->m_RayArea.GetCharacters());
    ////for (auto chr : chrs)
    ////{
    ////    // ���� ĳ������ �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
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
