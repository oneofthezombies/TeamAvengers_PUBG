#include "stdafx.h"
#include "Character.h"
#include "SkinnedMeshController.h"

Character::WaistRotation::WaistRotation(const float limit, const float factor)
    : LIMIT_OF_ANGLE(limit)
    , QUANTITY_FACTOR(factor)
    , m_Angle(0.0f)
{
}

Character::RootTransform::RootTransform(const float moveSpeed)
    : MOVE_SPEED(moveSpeed)
    , m_Direction(0.0f, 0.0f, -1.0f)
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

void Character::updateTransform()
{
    pSkinnedMeshController->Update([this]() 
    {
        // modify local bones
        D3DXMATRIX rx;
        D3DXMatrixRotationX(&rx, m_waistRotation.m_Angle);
        m_framePtr.pWaist->TransformationMatrix *= rx;

        // for root motion animation
        m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
    });
}

void Character::rotateWaist(const float quantity)
{
    auto& wr = m_waistRotation;

    wr.m_Angle += quantity;

    if (wr.m_Angle < -wr.LIMIT_OF_ANGLE)
        wr.m_Angle = -wr.LIMIT_OF_ANGLE;
    else if (wr.m_Angle > wr.LIMIT_OF_ANGLE)
        wr.m_Angle = wr.LIMIT_OF_ANGLE;
}

int Character::GetIndex() const
{
    return m_index;
}

TAG_COLLISION Character::GetTagCollisionBody(const int index)
{
    switch (index)
    {
    case 0:
        return TAG_COLLISION::BODY_OF_PLAYER_1;
    case 1:
        return TAG_COLLISION::BODY_OF_PLAYER_2;
    case 2:
        return TAG_COLLISION::BODY_OF_PLAYER_3;
    case 3:
        return TAG_COLLISION::BODY_OF_PLAYER_4;
    default:
        {
            assert(false &&
                "Character::GetTagCollisionBody() failed. index is wrong.");
            return TAG_COLLISION::IDLE;
        }
    }
}

TAG_COLLISION Character::GetTagCollisionDamage(const int index)
{
    switch (index)
    {
    case 0:
        return TAG_COLLISION::DAMAGE_OF_PLAYER_1;
    case 1:
        return TAG_COLLISION::DAMAGE_OF_PLAYER_2;
    case 2:
        return TAG_COLLISION::DAMAGE_OF_PLAYER_3;
    case 3:
        return TAG_COLLISION::DAMAGE_OF_PLAYER_4;
    default:
        {
            assert(false &&
                "Character::GetTagCollisionDamage() failed. index is wrong.");
            return TAG_COLLISION::IDLE;
        }
    }
}
