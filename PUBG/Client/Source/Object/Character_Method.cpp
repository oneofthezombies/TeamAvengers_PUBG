#include "stdafx.h"
#include "Character.h"
#include "SkinnedMeshController.h"
#include "CharacterPart.h"
#include "DirectionalLight.h"

Character::WaistRotation::WaistRotation(const float limit, const float factor)
    : LIMIT_OF_ANGLE(limit)
    , QUANTITY_FACTOR(factor)
    , m_angle(0.0f)
{
}

Character::RootTransform::RootTransform(const float moveSpeed)
    : MOVE_SPEED(moveSpeed)
{
}
Character::Info::Info()
    : pTransform(nullptr)
    , pRotationForCamera(nullptr)
    , pFPP(nullptr)
    , pTPP(nullptr)
{
}

Character::FramePtr::FramePtr()
    : pRoot(nullptr)
    , pWaist(nullptr)
    , pHandGun(nullptr)
    , pTPP(nullptr)
    , pFPP(nullptr)
{
}

void Character::setFramePtr()
{
    m_framePtr.pWaist = pSkinnedMeshController->FindFrame("spine_01");
    m_framePtr.pRoot = pSkinnedMeshController->FindFrame("root");
    m_framePtr.pHandGun = pSkinnedMeshController->FindFrame("ik_hand_gun");
    m_framePtr.pTPP = pSkinnedMeshController->FindFrame("camera_tpp");
    m_framePtr.pFPP = pSkinnedMeshController->FindFrame("camera_fpp");
}

void Character::subscribeCollisionEvent()
{
    if (isMine())
    {
        auto tagBody = GetTagCollisionBody(m_index);
        for (int i = 0; i < NUM_PLAYER; ++i)
        {
            if (i == m_index) continue;

            Collision()()->SubscribeCollisionEvent(
                GetTagCollisionDamage(i), tagBody);
        }
    }
}

bool Character::isMine() const
{
    return m_index == Communication()()->m_MyInfo.m_ID;
}

void Character::setAnimation(
    const TAG_ANIM_CHARACTER tag, const bool isBlend, 
    const float currentWeight, const float nextWeight, const float blendTime)
{
    assert(pSkinnedMeshController && 
        "Character::setAnimation(), skinned mesh controller is null.");

    pSkinnedMeshController->SetAnimation(
        TagAnimToString::Get(tag), 
        isBlend, 
        currentWeight, 
        nextWeight, 
        blendTime);

    m_animState = tag;
}

void Character::setInfo()
{
    m_info.pTransform = GetTransform();
    m_info.pRotationForCamera = &m_rotationForCamera;
    m_info.pTPP = m_framePtr.pTPP;
    m_info.pFPP = m_framePtr.pFPP;
}

D3DXVECTOR3 Character::getForward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::RIGHT, rot);
    dir.x *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getUp()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::UP, rot);
    //dir.y *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

void Character::updateBone()
{
    // modify local bones
    D3DXMATRIX rx;
    D3DXMatrixRotationX(&rx, m_waistRotation.m_angle);
    m_framePtr.pWaist->TransformationMatrix *= rx;

    // for root motion animation
    m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
}

void Character::updateDependency()
{
    GetTransform()->Update();
    pSkinnedMeshController->UpdateAnimation();
    updateBone();
    pSkinnedMeshController->UpdateModel();
    updateTotalInventory();
    if (m_pRootCharacterPart) m_pRootCharacterPart->Update();

    pSkinnedMeshController->Render(
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World, 
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });

    if (m_pRootCharacterPart) m_pRootCharacterPart->Render();
}

void Character::communicate()
{
    //if (isMine())
    //{
    //    if (isFired)
    //    {
    //        D3DXQUATERNION rot;
    //        D3DXQuaternionRotationAxis(&rot, &Vector3::UP, D3DX_PI * 0.5f);
    //        auto bullet = BulletPool()()->Fire(Matrix::GetTranslation(
    //            m_framePtr.pHandGun->CombinedTransformationMatrix
    //            * tr->GetTransformationMatrix()),
    //            rot, 0.1f, 10.0f, GetTagCollisionDamage(m_index));
    //        pCom->SendEventFireBullet(bullet);
    //    }

    //    if (isUpdated)
    //        pCom->SendPosition(pos);
    //}
}

void Character::rotateWaist(const float quantity)
{
    auto& wr = m_waistRotation;

    wr.m_angle += quantity;

    if (wr.m_angle < -wr.LIMIT_OF_ANGLE)
        wr.m_angle = -wr.LIMIT_OF_ANGLE;
    else if (wr.m_angle > wr.LIMIT_OF_ANGLE)
        wr.m_angle = wr.LIMIT_OF_ANGLE;
}

int Character::GetIndex() const
{
    return m_index;
}

TAG_COLLISION Character::GetTagCollisionBody(const int index)
{
    switch (index) {
    case 0: return TAG_COLLISION::Player_0_Body;
    case 1: return TAG_COLLISION::Player_1_Body;
    case 2: return TAG_COLLISION::Player_2_Body;
    case 3: return TAG_COLLISION::Player_3_Body;
    default: {
            assert(false &&
                "Character::GetTagCollisionBody() failed. index is wrong.");
            return TAG_COLLISION::Idle;
        }
    }
}

TAG_COLLISION Character::GetTagCollisionDamage(const int index)
{
    switch (index) {
    case 0: return TAG_COLLISION::Player_0_Damage;
    case 1: return TAG_COLLISION::Player_1_Damage;
    case 2: return TAG_COLLISION::Player_2_Damage;
    case 3: return TAG_COLLISION::Player_3_Damage;
    default: {
            assert(false &&
                "Character::GetTagCollisionDamage() failed. index is wrong.");
            return TAG_COLLISION::Idle;
        }
    }
}
