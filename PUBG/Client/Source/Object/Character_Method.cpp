#include "stdafx.h"
#include "Character.h"
#include "SkinnedMeshController.h"

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

Character::FramePtr::FramePtr()
    : pRoot(nullptr)
    , pWaist(nullptr)
    , pHandGun(nullptr)
{
}

void Character::setFramePtr()
{
    m_framePtr.pWaist = pSkinnedMeshController->FindFrame("spine_01");
    m_framePtr.pRoot = pSkinnedMeshController->FindFrame("root");
    m_framePtr.pHandGun = pSkinnedMeshController->FindFrame("ik_hand_gun");
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

D3DXVECTOR3 Character::getForward()
{
    auto rot = GetTransform()->GetRotation() * m_rotationOffset;
    auto dir = Vector3::Rotate(Vector3::FORWARD, rot);
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getRight()
{
    auto rot = GetTransform()->GetRotation() * m_rotationOffset;
    auto dir = Vector3::Rotate(Vector3::RIGHT, rot);
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

void Character::updateTransform()
{
    pSkinnedMeshController->Update([this]() 
    {
        // modify local bones
        D3DXMATRIX rx;
        D3DXMatrixRotationX(&rx, m_waistRotation.m_angle);
        m_framePtr.pWaist->TransformationMatrix *= rx;

        // for root motion animation
        m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
    });
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
    switch (index)
    {
    case 0: return TAG_COLLISION::Player_0_Body;
    case 1: return TAG_COLLISION::Player_1_Body;
    case 2: return TAG_COLLISION::Player_2_Body;
    case 3: return TAG_COLLISION::Player_3_Body;
    default:
        {
            assert(false &&
                "Character::GetTagCollisionBody() failed. index is wrong.");
            return TAG_COLLISION::Idle;
        }
    }
}

TAG_COLLISION Character::GetTagCollisionDamage(const int index)
{
    switch (index)
    {
    case 0: return TAG_COLLISION::Player_0_Damage;
    case 1: return TAG_COLLISION::Player_1_Damage;
    case 2: return TAG_COLLISION::Player_2_Damage;
    case 3: return TAG_COLLISION::Player_3_Damage;
    default:
        {
            assert(false &&
                "Character::GetTagCollisionDamage() failed. index is wrong.");
            return TAG_COLLISION::Idle;
        }
    }
}
