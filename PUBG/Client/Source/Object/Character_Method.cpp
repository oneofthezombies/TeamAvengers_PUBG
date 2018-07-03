#include "stdafx.h"
#include "Character.h"
#include "SkinnedMeshController.h"
#include "CharacterPart.h"
#include "DirectionalLight.h"
#include "AnimationState.h"
#include "Item.h"
#include "ItemInfo.h"

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
Character::IsPressing::IsPressing()
    : _LAlt(false)
    , _LCtrl(false)
    , _LShift(false)
    , _W(false)
    , _S(false)
    , _A(false)
    , _D(false)
{
}

bool Character::IsPressing::operator==(const IsPressing& other) const
{
    if (_LAlt != other._LAlt) return false;
    if (_LCtrl != other._LCtrl) return false;
    if (_LShift != other._LShift) return false;
    if (_W != other._W) return false;
    if (_S != other._S) return false;
    if (_A != other._A) return false;
    if (_D != other._D) return false;

    return true;
}

bool Character::IsPressing::operator!=(const IsPressing& other) const
{
    return !(*this == other);
}

Character::IsPressed::IsPressed()
    : _Z(false)
    , _X(false)
    , _C(false)
    , _R(false)
    , _Space(false)
    , _Num1(false)
    , _Num2(false)
    , _Num3(false)
    , _Num4(false)
    , _Num5(false)
    , _LButton(false)
    , _RButton(false)
{

}

Character::FramePtr::FramePtr()
    : pRoot(nullptr)
    , pWaist(nullptr)
    , pHandGun(nullptr)
    , pTPP(nullptr)
    , pFPP(nullptr)
    , pSlotPrimary(nullptr)
    , pSlotSecondary(nullptr)
    , pSlotMelee(nullptr)
    , pSlotThrowable(nullptr)
{
}

Character::IsJumping::IsJumping()
    : isJumping(false)
    
    , jumpPower(15.0f)
    , gravity(0.5f)
    , currGravity(0.0f)
    , maxStepHeight(5.f)
{
}

void Character::setFramePtr()
{
    m_framePtr.pWaist = pSkinnedMeshController->FindFrame("spine_01");
    m_framePtr.pRoot = pSkinnedMeshController->FindFrame("root");
    m_framePtr.pHandGun = pSkinnedMeshController->FindFrame("ik_hand_gun");
    m_framePtr.pTPP = pSkinnedMeshController->FindFrame("camera_tpp");
    m_framePtr.pFPP = pSkinnedMeshController->FindFrame("camera_fpp");
    m_framePtr.pSlotPrimary = pSkinnedMeshController->FindFrame("slot_primary");
    m_framePtr.pSlotSecondary = pSkinnedMeshController->FindFrame("slot_secondary");
    m_framePtr.pSlotMelee = pSkinnedMeshController->FindFrame("slot_melee");
    m_framePtr.pSlotThrowable = pSkinnedMeshController->FindFrame("slot_throwable");
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

void Character::handleInput(IsPressing* OutIsPressing)
{
    assert(OutIsPressing && "Character::handleInput(IsPressing), argument is null.");

    InputManager* pInput = Input()();

    OutIsPressing->_LAlt   = pInput->IsStayKeyDown(VK_LMENU);
    OutIsPressing->_LCtrl  = pInput->IsStayKeyDown(VK_LCONTROL);
    OutIsPressing->_LShift = pInput->IsStayKeyDown(VK_LSHIFT);
    OutIsPressing->_W      = pInput->IsStayKeyDown('W');
    OutIsPressing->_S      = pInput->IsStayKeyDown('S');
    OutIsPressing->_A      = pInput->IsStayKeyDown('A');
    OutIsPressing->_D      = pInput->IsStayKeyDown('D');
}

void Character::handleInput(IsPressed* OutIsPressed)
{
    assert(OutIsPressed && "Character::handleInput(IsPressed), argument is null.");

    InputManager* pInput = Input()();

    OutIsPressed->_Z            = pInput->IsOnceKeyDown('Z');
    OutIsPressed->_X            = pInput->IsOnceKeyDown('X');
    OutIsPressed->_C            = pInput->IsOnceKeyDown('C');
    OutIsPressed->_R            = pInput->IsOnceKeyDown('R');
    OutIsPressed->_Space        = pInput->IsOnceKeyDown(VK_SPACE);
    OutIsPressed->_Num1         = pInput->IsOnceKeyDown('1');
    OutIsPressed->_Num2         = pInput->IsOnceKeyDown('2');
    OutIsPressed->_Num3         = pInput->IsOnceKeyDown('3');
    OutIsPressed->_Num4         = pInput->IsOnceKeyDown('4');
    OutIsPressed->_Num5         = pInput->IsOnceKeyDown('5');
    OutIsPressed->_LButton      = pInput->IsOnceKeyDown(VK_LBUTTON);
    OutIsPressed->_RButton      = pInput->IsOnceKeyDown(VK_RBUTTON);
}

void Character::setAttacking()
{
    //for Debug
    TAG_RES_STATIC tag;
    Item* hand = m_totalInventory.m_hand;
    if (hand)
    {
        Debug << "On hand Weapon: ";
        tag = hand->GetTagResStatic();
        Debug << ItemInfo::GetName(tag) << "\n";
    }
    Debug << "Attacking: " << ForDebugGetAttacking(m_attacking) << "\n";

    TotalInventory& inven = m_totalInventory; 

    if (hasFinishEvent()) return;

    if (m_currentOnceKey._Num1)
    {
        if (inven.m_weaponPrimary)
        {
            //등짝에 주무기가 있다
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_hand = inven.m_weaponPrimary;
                inven.m_weaponPrimary = nullptr;
                setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand, false, [this]() 
                {
                    setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);
                });
            }
            else if (m_attacking == Attacking::Rifle)
            {
                //보조무기를 해제하고, 주무기를 장착한다
                setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static, false, [this, &inven]() 
                {
                    inven.m_weaponSecondary = inven.m_hand;

                    inven.m_hand = inven.m_weaponPrimary;
                    inven.m_weaponPrimary = nullptr;
                    setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand, false, [this]() 
                    {                        
                        setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);
                    });
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
            inven.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(inven.m_hand->GetTagResStatic());
        }
    }
    else if (m_currentOnceKey._Num2)
    {
        if (inven.m_weaponSecondary)
        {
            //등짝에 보조무기가 있다
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_hand = inven.m_weaponSecondary;
                inven.m_weaponSecondary = nullptr;
                setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand, false, [this]()
                {
                    setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);
                });
            }
            else if (m_attacking == Attacking::Rifle)
            {
                //주무기를 해제하고 보조무기를 장착한다
                setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static, false, [this, &inven]()
                {
                    inven.m_weaponPrimary = inven.m_hand;

                    inven.m_hand = inven.m_weaponSecondary;
                    inven.m_weaponSecondary = nullptr;
                    setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand, false, [this]()
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);
                    });
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
            inven.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(inven.m_hand->GetTagResStatic());
        }
    }
    //for test 'X' - 다시 X누르면 이전 무기 장착되는 것도 해야함
    else if (m_currentOnceKey._X)
    {
        //손에 무기를 들고 있다면 해제한다
        //무기가 주무기냐, 보조무기냐에 따라서 다른 애니메이션을 실행한다. 우선 QBZ 주무기 Kar98k 보조무기
        if (inven.m_hand)
        {
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic();
            if (tag == TAG_RES_STATIC::QBZ)
            {
                m_attacking = Attacking::Unarmed;
                //주무기를 해제한다
                setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static, false, [this, &inven]()
                {
                    inven.m_weaponPrimary = inven.m_hand;
                    inven.m_hand = nullptr;
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false); //TODO: 해제는 되는데 다리가 부자연스러움
                });

            }
            else if (tag == TAG_RES_STATIC::Kar98k)
            {
                m_attacking = Attacking::Unarmed;
                //보조무기를 해제한다
                setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static, false, [this, &inven]()
                {
                    inven.m_weaponSecondary = inven.m_hand;
                    inven.m_hand = nullptr;
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);
                });
            }
        }
    }
}

void Character::setStance()
{
    //for Debug
    Debug << "Stance: " << ForDebugGetStance(m_stance) << "\n";

    if (m_currentOnceKey._C)
    {
        if (m_stance == Stance::Crouch)
        {
            // TODO : check obstacle

            m_stance = Stance::Stand;
        }
        else
        {
            // TODO : check obstacle

            m_stance = Stance::Crouch;
        }
    }
    else if (m_currentOnceKey._Z)
    {
        if (m_stance == Stance::Prone)
        {
            // TODO : check obstacle

            m_stance = Stance::Stand;
        }
        else
        {
            // TODO : check obstacle

            m_stance = Stance::Prone;
        }
    }
}

void Character::setReload()
{
    TotalInventory& inven = m_totalInventory;

    //R버튼을 눌렀는데,
    //1. 총을 들고 있고 
    //2. 총에 알맞은 총알이 있다면 
    //3. 갖고있는 총알 개수 내에서 (장탄수-현재 장전된 총알개수) 만큼 총알을 장전해준다
    if (m_currentOnceKey._R)
    {
        if (inven.m_hand)
        {
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic(); //총 종류
            TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag); //탄약 종류
            int magSize = ItemInfo::GetMagazineSize(tag);         //장탄 수
            int numBulletCurrentLoad = inven.m_hand->GetNumBullet(); //장전되어있는 총알 수

            if (numBulletCurrentLoad == magSize) //이미 가득 장전 되어있는 경우
                return;
        
            //총에 알맞는 총알이 있는지 확인해서 장전
            auto it = inven.m_mapInventory.find(ammoType);
            if (it != inven.m_mapInventory.end())
            {
                int numBulletInInventory = (*it).second.back()->GetCount(); //인벤토리에 있는 총알 수
                int numBulletNeedReload = magSize - numBulletCurrentLoad;   //장전할 총알 수 (장탄수 - 현재 장전된 총알 개수)

                cout << ItemInfo::GetName(ammoType);
                cout << "을 " << ItemInfo::GetName(tag) << "에 장전" << endl;
                cout << "인벤토리에 있는 총알 수: " << numBulletInInventory << "\n";

                inven.m_numReload = 0;
                if (numBulletInInventory >= numBulletNeedReload)
                {
                    inven.m_hand->SetNumBullet(numBulletNeedReload);
                    (*it).second.back()->SetCount(numBulletInInventory - numBulletNeedReload);

                    inven.m_numReload = numBulletNeedReload;
                }
                else
                {
                    inven.m_hand->SetNumBullet(numBulletInInventory);
                    (*it).second.back()->SetCount(0);

                    inven.m_numReload = numBulletInInventory;
                }
                cout << "장정된 총알 개수: " << inven.m_hand->GetNumBullet() << "\n";
                cout << "인벤토리에 남아있는 총알 개수: " << (*it).second.back()->GetCount() << "\n";

                /*
                TODO: 장전 애니메이션 추가 - Prone인 경우, fast reload
                */
                if (tag == TAG_RES_STATIC::QBZ)
                {
                    setAnimation(TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Base, false, [this]()
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);
                    });
                }
                else if (tag == TAG_RES_STATIC::Kar98k)
                {
                    inven.m_numReload = 4;

                    if (inven.m_numReload == 5)
                    {
                        // fast reload
                    }
                    else
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Kar98k_Reload_Start, false, std::bind(&Character::onKar98kReload, this));
                    }
                }
            }
            else
            {
                cout << "총알이 필요해" << endl;
                //do nothing
            }
        }
        else //inven.m_hand == nullptr
        {
            cout << "총을 장착해줘" << endl;
            //do nothing
        }
    }
}

void Character::cameraCharacterRotation(const float dt, D3DXQUATERNION* OutRotation)
{
    ICamera* pCurrentCamera = CurrentCamera()();

    // get mouse pos
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(g_hWnd, &mouse);

    POINT diff;
    diff.x = mouse.x - 1280 / 2;
    diff.y = mouse.y - 720 / 2;
    if (diff.x < 2 && diff.x > -2) diff.x = 0;
    if (diff.y < 2 && diff.y > -2) diff.y = 0;
    const float yaw = diff.x * 0.2f * dt;
    const float pitch = diff.y * 0.2f * dt;

    static bool tempBool = false;
    if (m_currentStayKey._LAlt)
    {
        if (pCurrentCamera)
        {
            if (pCurrentCamera->GetTagCamera() == TAG_CAMERA::Third_Person || pCurrentCamera->GetTagCamera() == TAG_CAMERA::Default)
            {
                m_rotationForCamera.x += -pitch;
                m_rotationForCamera.y += yaw;
                tempBool = true;
            }
        }
    }
    else // isPressing_LAlt == false
    {
        D3DXQUATERNION q;
        D3DXQuaternionRotationYawPitchRoll(&q, yaw, 0.0f, 0.0f);
        *OutRotation *= q;
        m_rotationForCamera.x += -pitch;
        // reset rotFotCameraTP
        if (tempBool)
        {
            m_rotationForCamera = Vector3::ZERO;
            tempBool = false;
        }
    }

    static bool test_sound = true;

    if (Input()()->IsOnceKeyDown(VK_LEFT))
        test_sound = true;
    if (Input()()->IsOnceKeyDown(VK_RIGHT))
    {
        test_sound = false;
    }
    if (test_sound)
    {
        POINT center;
        center.x = 1280 / 2;
        center.y = 720 / 2;
        ClientToScreen(g_hWnd, &center);
        SetCursorPos(center.x, center.y);
    }

}

void Character::animationMovementControl(D3DXVECTOR3* OutPosition, TAG_ANIM_CHARACTER* OutTag)
{
    Direction direction;
    Moving    moving;

    float movingFactor;
    if (m_currentOnceKey._Space)
        m_Jump.isJumping = true;

    //Moving 3개 -----------------------------------------------------
    if (m_currentStayKey._LShift && !m_currentStayKey._LCtrl)
    {
        moving = Moving::Sprint;
        movingFactor = 2.0f;
    }
    else if (m_currentStayKey._LCtrl && !m_currentStayKey._LShift)
    {
        moving = Moving::Walk;
        movingFactor = 0.5f;
    }
    else
    {
        moving = Moving::Run;
        movingFactor = 1.0f;
    }

    //Direction 8개 -----------------------------------------------------
    if (m_currentStayKey._W&&m_currentStayKey._D)
    {
        direction = Direction::FrontRight;
        *OutPosition += getForwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._D&&m_currentStayKey._S)
    {
        direction = Direction::BackRight;
        *OutPosition += getBackwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._S&&m_currentStayKey._A)
    {
        direction = Direction::BackLeft;
        *OutPosition += getBackwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._A&&m_currentStayKey._W)
    {
        direction = Direction::FrontLeft;
        *OutPosition += getForwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._W)
    {
        direction = Direction::Front;
        *OutPosition += getForward() * movingFactor * m_rootTransform.MOVE_SPEED;

    }
    else if (m_currentStayKey._D)
    {
        direction = Direction::Right;
        *OutPosition += getRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._S)
    {
        direction = Direction::Back;
        //*pOut += getForward() * -1.0f;
        *OutPosition += getBackward() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._A)
    {
        direction = Direction::Left;
        *OutPosition += getLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else
    {
        direction = Direction::StandStill;
    }

    if (OutTag) //if null, no changes in animation
    {
        if (m_Jump.isJumping&&m_currentStayKey._W)
            *OutTag = TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F;
        else if (m_Jump.isJumping)
            *OutTag = TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary;
        else
            *OutTag = AnimationState::Get(m_attacking, m_stance, moving, direction);
    }
}
bool Character::isMine() const
{
    return m_index == Communication()()->m_MyInfo.m_ID;
}

void Character::setAnimation(
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float blendTime,
    const float nextWeight)
{
    assert(pSkinnedMeshController && 
        "Character::setAnimation(), skinned mesh controller is null.");

    pSkinnedMeshController->SetAnimation(
        TagAnimation::GetString(tag), 
        TagAnimation::GetSpeed(tag),
        isBlend,
        blendTime,
        nextWeight);
}

void Character::applyTarget_Y_Position(OUT D3DXVECTOR3 * pOut)
{
    
    IScene* pCurrentScene = CurrentScene()();
    if (!pCurrentScene)
        assert(false && "No CurrentScene");


    D3DXVECTOR3 targetPos = *pOut;
    float height = 0;
    bool isIntersected = true;

    if (m_Jump.isJumping)
    {

        targetPos.y += m_Jump.jumpPower - m_Jump.currGravity;
        m_Jump.currGravity += m_Jump.gravity;
        
        isIntersected = pCurrentScene->GetHeight(targetPos, &height);

        //if (!isIntersected)
        //{
        //    // Do nothing just yet

        //}
        //else
        //{

        //}

        if (targetPos.y <= height && m_Jump.jumpPower < m_Jump.currGravity)
        {
            targetPos.y = height;
            m_Jump.isJumping = false;
            m_Jump.currGravity = 0.0f;
        }
    }
    else //when no jump
    {
        isIntersected = pCurrentScene->GetHeight(targetPos, &height);
        if (!isIntersected /*|| fabs(height - *pOut.y) > m_Jump.maxStepHeight*/)
        {
            //Do nothing
        }
        else
        {
            targetPos.y = height;
        }

    }

    *pOut = targetPos;
}

void Character::rifleShooting()
{
    m_totalInventory.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(m_totalInventory.m_hand->GetTagResStatic());
    m_totalInventory.m_numReload--;
    //Goal : get Fire starting position , get fire direction
    m_totalInventory.m_hand->UpdateModel(); //update to get position of frame "gun_bolt" 
    D3DXMATRIX mat =
        m_totalInventory.m_hand->GetGunBolt()->CombinedTransformationMatrix  //model space combinde matrix
        * m_framePtr.pHandGun->CombinedTransformationMatrix // hand gun space matrix
        * GetTransform()->GetTransformationMatrix();    //character world matrix
    
    D3DXVECTOR3 bulletFirePos = D3DXVECTOR3(mat._41, mat._42, mat._43); 
    
    D3DXVECTOR3 bulletDir;
    CurrentCamera()()->CalcPickedPosition(&bulletDir, 1280 / 2, 720 / 2);
    bulletDir = bulletDir - bulletFirePos;
    D3DXVec3Normalize(&bulletDir, &bulletDir);
    //-------------------------


    switch (m_totalInventory.m_hand->GetTagResStatic())
    {
    case TAG_RES_STATIC::QBZ:
        BulletPool()()->Fire(bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::QBZ), ItemInfo::GetBaseDamage(TAG_RES_STATIC::QBZ), TAG_COLLISION::Impassable);
        break;
    case TAG_RES_STATIC::Kar98k:
        BulletPool()()->Fire(bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::Kar98k), ItemInfo::GetBaseDamage(TAG_RES_STATIC::Kar98k), TAG_COLLISION::Impassable);
        break;
    }

}

bool Character::hasFinishEvent() const
{
    assert(pSkinnedMeshController &&
        "Character::hasFinishEvent(), skinned mesh controller is null.");

    return pSkinnedMeshController->HasFinishEvent();
}

void Character::setAnimation(
    const TAG_ANIM_CHARACTER tag, 
    const bool isBlend, 
    const std::function<void()>& finishEvent)
{
    assert(pSkinnedMeshController &&
        "Character::setAnimation(), skinned mesh controller is null.");

    pSkinnedMeshController->SetAnimation(
        TagAnimation::GetString(tag),
        TagAnimation::GetSpeed(tag),
        isBlend,
        0.3f,
        0.0f,
        finishEvent);

    m_animState = tag;
}

void Character::setInfo()
{
    m_info.pTransform = GetTransform();
    m_info.pRotationForCamera = &m_rotationForCamera;
    m_info.pTPP = m_framePtr.pTPP;
    m_info.pFPP = m_framePtr.pFPP;
}

D3DXVECTOR3 Character::getUp()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::UP, rot);
    //dir.y *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getForward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getBackward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD, rot);
    dir.z *= -1.0f; 
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::LEFT, rot);
    dir.x *= -1.0f;
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

D3DXVECTOR3 Character::getForwardLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD_RIGNT, rot);//왜 forward_Right인지..?? 이래야 잘 돌아 가는 이유는?
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getForwardRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD_LEFT, rot); //왜 forward_left인지..?? 이래야 잘 돌아 가는 이유는?
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}



D3DXVECTOR3 Character::getBackwardRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD_LEFT, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getBackwardLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD_RIGHT, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}



void Character::updateBone()
{
    // modify local bones
    D3DXMATRIX rx;
    D3DXMatrixRotationX(&rx, m_waistRotation.m_angle);
    m_framePtr.pWaist->TransformationMatrix *= rx;

    //TODO 애니메이션 다시 돌아오게 하기
    // for root motion animation
    m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
}

void Character::updateDependency()
{
    // update
    GetTransform()->Update();
    pSkinnedMeshController->UpdateAnimation();
    updateBone();
    pSkinnedMeshController->UpdateModel();
    updateTotalInventory();
    if (m_pRootCharacterPart) m_pRootCharacterPart->Update();

    // render
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
    renderTotalInventory();
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

