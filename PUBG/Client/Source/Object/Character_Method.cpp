#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "CharacterPart.h"
#include "Light.h"
#include "AnimationState.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
#include "HeightMap.h"
#include "ScenePlay.h"
#include "Ballistics.h"
#include "TerrainFeature.h"
#include "DeathDropBox.h"
#include "UIText.h"
#include "UIImage.h"

const float MovingFactor::UNARMED_RUN = 180.0f;
const float MovingFactor::UNARMED_SPRINT = 260.0f;
const float MovingFactor::UNARMED_WALK = 120.0f;
const float MovingFactor::RIFLE_RUN = 120.0f;
const float MovingFactor::RIFLE_SPRINT = 200.0f;
const float MovingFactor::RIFLE_WALK = 100.0f;
const float Character::RootTransform::DEBUG_MOVE_SPEED = 5.0f;

Character::WaistRotation::WaistRotation(const float limit/*, const float factor*/)
    : LIMIT_OF_ANGLE(limit)
    //, QUANTITY_FACTOR(factor)
    , m_angle(0.0f)
    , m_limit(false)
{
}

Character::HeadRotation::HeadRotation(const float limit/*, const float factor*/)
    : LIMIT_OF_ANGLE(limit)
    //, QUANTITY_FACTOR(factor)
    , m_angle(0.0f)
{
}

//Character::ArmRotation::ArmRotation(const float limit, const float factor)
//    :LIMIT_OF_ANGLE(limit)
//    , QUANTITY_FACTOR(factor)
//    , m_angle(0.0f)
//{
//}

Character::RootTransform::RootTransform(const float moveSpeed)
    : MOVE_SPEED(moveSpeed)
{
}
Character::Info::Info()
    : pTransform(nullptr)
    , pRotationForCamera(nullptr)
    , pHead(nullptr)
    , pFPP(nullptr)
    , pTPP(nullptr)
    , pHandGun(nullptr)
    , pGunBolt(nullptr)
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
    , _F(false)
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

FramePtr::FramePtr()
    : pRoot(nullptr)
    , pHead(nullptr)
    , pWaist(nullptr)
    , pHandGun(nullptr)
    , pTPP(nullptr)
    , pFPP(nullptr)
    , pSlotPrimary(nullptr)
    , pSlotSecondary(nullptr)
    , pSlotMelee(nullptr)
    , pSlotThrowable(nullptr)
    , pLeftClavicle(nullptr)
    , pRightClavicle(nullptr)
{
}

Character::IsJumping::IsJumping()
    : isJumping(false)
    
    , jumpPower(12.0f)
    , gravity(0.6f)
    , currGravity(0.0f)
    , maxStepHeight(5.0f)
{
}

Character::State::State()
    : position(Vector3::ZERO)
    , rotation(Vector3::ZERO)
    , isHeadBump(false)
{
}

void Character::setFramePtr()
{
    m_framePtr.pWaist         = pAnimation->FindFrame("spine_01");
    m_framePtr.pRoot          = pAnimation->FindFrame("root");
    m_framePtr.pHead          = pAnimation->FindFrame("head");
    m_framePtr.pLeftClavicle  = pAnimation->FindFrame("clavicle_l");
    m_framePtr.pRightClavicle = pAnimation->FindFrame("clavicle_r");

    m_framePtr.pHandGun       = pAnimation->FindFrame("item_r");

    m_framePtr.pTPP           = pAnimation->FindFrame("camera_tpp");
    m_framePtr.pFPP           = pAnimation->FindFrame("camera_fpp");
    m_framePtr.pSlotPrimary   = pAnimation->FindFrame("slot_primary");
    m_framePtr.pSlotSecondary = pAnimation->FindFrame("slot_secondary");
    m_framePtr.pSlotMelee     = pAnimation->FindFrame("slot_melee");
    m_framePtr.pSlotThrowable = pAnimation->FindFrame("slot_throwable");

    FramePtr& p = m_framePtr;
    assert(
        p.pWaist &&
        p.pRoot &&
        p.pHead &&
        p.pLeftClavicle &&
        p.pRightClavicle &&
        p.pHandGun &&
        p.pTPP &&
        p.pFPP &&
        p.pSlotPrimary &&
        p.pSlotSecondary &&
        p.pSlotMelee &&
        p.pSlotThrowable && "Character::setFramePtr(), pointer is null.");
}

void Character::subscribeCollisionEvent()
{
    if (IsMine())
    {
        auto tagBody = GetTagCollisionBody(m_index);
        for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
        {
            if (i == m_index) continue;

            Collision()()->AddCollisionEvent(
                GetTagCollisionDamage(i), 
                tagBody);
        }
    }
}

void Character::checkDead()
{
    if (m_health <= 0.0f)
    {
        //기절 애니메이션
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand)
            tagAnim = TAG_ANIM_CHARACTER::DBNO_Enter;
        else if (m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::DBNO_Enter_From_Crouch;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::DBNO_Enter_From_Prone;

        assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::updateMine(), tagAnim is COUNT");

        setAnimation(
            CharacterAnimation::BodyPart::BOTH,
            tagAnim,
            true,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f,
            [this]() {
            setAnimation(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::DBNO_Idle,
                true,
                0.3f);
        });

        Communication()()->SendIsDead(m_index, true);
        m_isDead = true;
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
    OutIsPressing->_LButton = pInput->IsStayKeyDown(VK_LBUTTON);
}

void Character::handleInput(IsPressed* OutIsPressed)
{
    assert(OutIsPressed && "Character::handleInput(IsPressed), argument is null.");

    InputManager* pInput = Input()();

    OutIsPressed->_Z            = pInput->IsOnceKeyDown('Z');
    OutIsPressed->_X            = pInput->IsOnceKeyDown('X');
    OutIsPressed->_C            = pInput->IsOnceKeyDown('C');
    OutIsPressed->_R            = pInput->IsOnceKeyDown('R');
    OutIsPressed->_F            = pInput->IsOnceKeyDown('F');
    OutIsPressed->_B            = pInput->IsOnceKeyDown('B');
    OutIsPressed->_Space        = pInput->IsOnceKeyDown(VK_SPACE);
    OutIsPressed->_Num1         = pInput->IsOnceKeyDown('1');
    OutIsPressed->_Num2         = pInput->IsOnceKeyDown('2');
    OutIsPressed->_Num3         = pInput->IsOnceKeyDown('3');
    OutIsPressed->_Num4         = pInput->IsOnceKeyDown('4');
    OutIsPressed->_Num5         = pInput->IsOnceKeyDown('5');
    OutIsPressed->_LButton      = pInput->IsOnceKeyDown(VK_LBUTTON);
    OutIsPressed->_RButton      = pInput->IsOnceKeyDown(VK_RBUTTON);
    OutIsPressed->_Tab          = pInput->IsOnceKeyDown(VK_TAB);
}

void Character::handleMouse(const float dt, MouseInput* mouseInput)
{
    // get mouse pos
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(g_hWnd, &mouse);

    POINT diff;
    diff.x = mouse.x - 1280 / 2;
    diff.y = mouse.y - 720 / 2;
    if (diff.x < 2 && diff.x > -2) diff.x = 0;
    if (diff.y < 2 && diff.y > -2) diff.y = 0;

    mouseInput->yaw = diff.x * 0.2f * dt;
    mouseInput->pitch = diff.y * 0.2f * dt;


    POINT center;
    center.x = 1280 / 2;
    center.y = 720 / 2;
    ClientToScreen(g_hWnd, &center);
    SetCursorPos(center.x, center.y);

}

void Character::backAction(D3DXQUATERNION* OutRotation, int virtical, int horizontal)
{
    //기본 수직 수평 반동 값
    float virtical_result;
    float horizontal_result;
    int Min = static_cast<int>(virtical * 0.5f);
    float Weight = 0.0005f;

    virtical_result = (virtical - (rand() % Min)) * Weight;                     //최소 값이 총의 수직반동의 반
    horizontal_result = ((horizontal) / 2 - (rand() % horizontal))*Weight;      // 0이 나올수 있음


    D3DXQUATERNION q;
    D3DXQuaternionRotationYawPitchRoll(&q, horizontal_result, 0.0f, 0.0f);

    //m_backAction.afterY = *OutRotation *q;
    //m_backAction.afterX = m_rotationForCamera.x + virtical_result;
    //나중에 값 바꾸면 디버그 확인용

    m_backAction.Ing = true;
    m_backAction.Rot = OutRotation;         //로테이션 값 바꿔주는거 저장 값이었지만 다른 코드가 바뀌면서 무쓸모해짐.
    m_backAction.curValY = horizontal_result;
    m_backAction.valY = horizontal_result;
    m_backAction.curValX = virtical_result;
    m_backAction.valX = virtical_result;
}

void Character::terrainFeaturesCollisionInteraction(OUT State* destState)
{
    m_adjust_Y_onCollision = 0.0f;
    m_isCollidedWithBox = false;
    IScene* pCurrentScene = CurrentScene()();
    bool hasCollision = false;
    const D3DXVECTOR3 currPos = GetTransform()->GetPosition();
    const D3DXVECTOR3 boxPos = m_bBox.center + m_bBox.position;
    D3DXVECTOR3 destDir = destState->position - currPos;

    auto tfs(pCurrentScene->m_NearArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        // 바운딩스피어가 충돌되지 않으면 다음 터레인피처와 충돌을 검사한다.
        if (!Collision::HasCollision(m_boundingSphere, tf->GetBoundingSphere())) continue;

        GetBoundingSphere().isRender = true;
        tf->GetBoundingSphere().isRender = true; // for recording

        for (auto& others : tf->GetBoundingBoxes())
        {
            // sliding vector
            if (Collision::HasCollision(m_bBox, others))
            {
                m_bBox.isRender = true;
                others.isRender = true;

                hasCollision = true;

                //부딛힌 면의 normal 값 구하기
                const D3DXVECTOR3 normal = Collision::GetCollidedNormal2(boxPos, others);

                //box로 걸어가면 sliding vector 적용
                if (D3DXVec3Dot(&destDir, &-normal) > 0.0f)
                {
                    destDir -= D3DXVec3Dot(&destDir, &normal) * normal;
                }
            }
        }
    }

    // collision with other characters
    for (auto o : pCurrentScene->m_NearArea.GetCharacters())
    {
        if (o->GetIndex() == m_index) continue;

        // 바운딩스피어가 충돌되지 않으면 다음 캐릭터와 충돌을 검사한다.
        if (!Collision::HasCollision(m_boundingSphere, o->GetBoundingSphere())) continue;

        // sliding vector
        const BoundingBox& others = o->GetBoundingBox();
        if (Collision::HasCollision(m_bBox, others))
        {
            hasCollision = true;

            //부딛힌 면의 normal 값 구하기
            const D3DXVECTOR3 normal = Collision::GetCollidedNormal2(boxPos, others);

            //box로 걸어가면 sliding vector 적용
            if (D3DXVec3Dot(&destDir, &-normal) > 0.0f)
            {
                destDir -= D3DXVec3Dot(&destDir, &normal) * normal;
            }
        }
    }
    // end collision /////////////////////////

    destState->position = currPos + destDir;

    Transform* tm = GetTransform();        // 만약 스프린트일 경우에는 쉬프트키 유무에 상관없이 런으로 바꾼다.
    if (destState->isHeadBump)
    {
        // TODO : impl
    }

    // update state
    tm->SetPosition(destState->position);
    tm->SetRotation(destState->rotation);

    // 이사하기 //NearArea(cell space)를 다시 구하기!
    std::size_t destCellIndex = pCurrentScene->GetCellIndex(destState->position);
    if (destCellIndex != m_cellIndex)
    {
        pCurrentScene->m_NearArea.CreateNearArea(destCellIndex);
        pCurrentScene->MoveCell(&m_cellIndex, destCellIndex, TAG_OBJECT::Character, this);
    }

    //m_adjust_Y_onCollision = destState->position.y;
    //m_isCollidedWithBox = hasCollision;
}
void Character::terrainFeaturesCollisionInteraction2(OUT State * destState)
{
    m_adjust_Y_onCollision = 0.0f;
    m_isCollidedWithBox = false;
    IScene* pCurrentScene = CurrentScene()();
    int hasCollision = 0;
    auto tfs(pCurrentScene->m_NearArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        if (hasCollision>0) break;

        // 바운딩스피어가 충돌되지 않으면 다음 터레인피처와 충돌을 검사한다.
        if (!Collision::HasCollision(m_boundingSphere, tf->GetBoundingSphere())) continue;


        for (auto& others : tf->GetBoundingBoxes())
        {
            if (hasCollision>1) break;

            if (Collision::HasCollision(m_bBox, others))
                hasCollision++;
            // sliding vector
            if (hasCollision>0)
            {
                //내가 원하는 방향의 단위 normal dir
                const D3DXVECTOR3 currPos = GetTransform()->GetPosition();
                D3DXVECTOR3 destDir(destState->position.x - currPos.x, destState->position.y - currPos.y, destState->position.z - currPos.z);
                D3DXVec3Normalize(&destDir, &destDir);

                //부딛힌 면의 normal 값 구하기
                D3DXVECTOR3 normal = Collision::GetCollidedNormal2(m_bBox.center + m_bBox.position, others);

                D3DXVECTOR3 towardsBoxDir = (others.position + others.center) - currPos;

                D3DXVec3Normalize(&towardsBoxDir, &towardsBoxDir);

                //box로 걸어가면 sliding vector 적용
                if (D3DXVec3Dot(&destDir, &-normal) > 0.0f)
                {
                    D3DXVECTOR3 slidingVector = destDir - D3DXVec3Dot(&destDir, &normal)*normal;
                    destState->position = currPos + slidingVector;
                }

            }
        }
    }

    //// collision with other characters
    //if (!hasCollision)
    //{
    //    for (auto o : pCurrentScene->m_NearArea.GetCharacters())
    //    {
    //        if (hasCollision) break;

    //        if (o->GetIndex() == m_index) continue;

    //        const D3DXVECTOR3 dist = destState->position - o->GetTransform()->GetPosition();
    //        const float distLen = D3DXVec3Length(&dist);
    //        if (distLen < RADIUS * 2.0f)
    //            hasCollision = true;
    //    }
    //}
    // end collision /////////////////////////
    Transform* tm = GetTransform();
    // 셋 커런트
    if (hasCollision>0)
    {
        // 만약 스프린트일 경우에는 쉬프트키 유무에 상관없이 런으로 바꾼다.
        if (destState->isHeadBump)
        {
            // TODO : impl
        }

        tm->SetPosition(destState->position);
    }
    else
    {
        // update state
        tm->SetPosition(destState->position);
        tm->SetRotation(destState->rotation);

        // 이사하기 //NearArea(cell space)를 다시 구하기!
        std::size_t destCellIndex = pCurrentScene->GetCellIndex(destState->position);
        if (destCellIndex != m_cellIndex)
        {
            pCurrentScene->m_NearArea.CreateNearArea(destCellIndex);
            pCurrentScene->MoveCell(&m_cellIndex, destCellIndex, TAG_OBJECT::Character, this);
            int i = 0;
        }
    }
    m_adjust_Y_onCollision = destState->position.y;
    if (hasCollision > 0)
        m_isCollidedWithBox = true;
}
void Character::itemSphereCollisionInteraction()
{
    //Item Spher와 character sphere 충돌 체크
    IScene * pCurrentScene = CurrentScene()();
    // clear dropped items
    auto& di = m_totalInventory.droppedItems;
    di.resize(0);

    auto itms(pCurrentScene->m_NearArea.GetItems());    //이 auto를 copy가 아닌 reference로 받는 방법은???

    m_inGameUI.pInteractionF->SetIsRender(false);

    // 보이는거 맴앞에
    for (auto itm : itms)
    {

        if (!Collision::HasCollision(m_boundingSphere, itm->GetBoundingSphere())) continue;
        //캐릭터와 Item의 spehre 가 충돌이 났다

        // UI로 F key가 나오게 하기 

        //Ray를 쏘아서 맞는 물건 먼저 먹기
        for (auto& rayItm : di)
        {
            Ray ray = Ray::RayAtWorldSpace(1280 / 2, 720 / 2);

            // 먼저 terrain features의 바운딩스피어와 충돌을 검사한다.
            BoundingSphere bs = rayItm->GetBoundingSphere();

            if (!D3DXSphereBoundProbe(
                &(bs.center + bs.position),
                bs.radius,
                &ray.m_pos,
                &ray.m_dir)) continue;
            string ItemName = ItemInfo::GetName(rayItm->GetTagResStatic());
            m_inGameUI.pInteractionText->SetText(ItemName);
            m_inGameUI.pInteractionF->SetIsRender(true);
            m_inGameUI.pInteractionBG->SetSize(D3DXVECTOR2(50.0f + 6.0f*ItemName.size(), 23.0f));
            m_inGameUI.pInteractionText->SetSize(D3DXVECTOR2(50.0f + 6.0f*ItemName.size(), 23.0f));

            if (m_currentOnceKey._F)
            {
                PutItemInTotalInventory(rayItm); //inventory에 넣기
            }
            return;
        }



        if (m_currentOnceKey._F)
        {
            PutItemInTotalInventory(itm); //inventory에 넣기
            return;
        }

        di.emplace_back(itm);
        if (di.size() >0)
        {
            string ItemName = ItemInfo::GetName(di[0]->GetTagResStatic());
            m_inGameUI.pInteractionText->SetText(ItemName);
            m_inGameUI.pInteractionF->SetIsRender(true);
            m_inGameUI.pInteractionBG->SetSize(D3DXVECTOR2(50.0f + 6.0f*ItemName.size(), 23.0f));
            m_inGameUI.pInteractionText->SetSize(D3DXVECTOR2(50.0f + 6.0f*ItemName.size(), 23.0f));
        }
    }

    auto deathDropboxes(pCurrentScene->m_NearArea.GetDeathDropBoxes());
    for (auto box : deathDropboxes)
    {
        if (!Collision::HasCollision(
            m_boundingSphere, 
            box->GetBoundingSphere())) continue;

        const auto& items = box->GetItems();
        di.insert(di.end(), items.begin(), items.end());
    }

    //캐릭터 spehre안에 아이템이 없으면 모션을 캔슬했다 (앞으로 문열기 등 interaction에서 문제가 많은 코드) (이후에 바뀔것이다)
    if (di.size() == 0)
        m_currentOnceKey._F = false;
}
//cout << virtical_result << "=============" << horizontal_result << endl;
void Character::characterRotation(MouseInput* mouseInput)
{
    ////케릭터 머리 Frame 움직이기
    rotateHead(-mouseInput->pitch);
    //캐릭터 허리 움직이기
    //if (!m_currentStayKey._LAlt)
    //    rotateWaist(mouseInput->yaw);
}

void Character::backActionFrame(D3DXQUATERNION* OutRotation)
{
    D3DXQUATERNION q;
    if (m_backAction.Up)
    {
        m_backAction.curValX = m_backAction.curValX * 0.5f;
        m_backAction.curValY = m_backAction.curValY * 0.5f;
        if (m_backAction.curValX < 0.0001f)
        {
            m_backAction.Up = false;
            return;
        }
        m_rotationForCamera.x += m_backAction.curValX;
        D3DXQuaternionRotationYawPitchRoll(&q, m_backAction.curValY, 0.0f, 0.0f);
        //*m_backAction.Rot *= q;       //예전코드에서 스트럭트에 저장된 주소에 바로 적용하였음.
        *OutRotation *= q;
    }
    else
    {
        m_backAction.curValX = m_backAction.curValX / 0.5f;
        m_backAction.curValY = m_backAction.curValY / 0.5f;
        if (m_backAction.curValX >= m_backAction.valX-0.001f)
        {
            m_backAction.Up = true;
            m_backAction.Ing = false;
            return;
        }
        m_rotationForCamera.x -= m_backAction.curValX;
        D3DXQuaternionRotationYawPitchRoll(&q, -m_backAction.curValY, 0.0f, 0.0f);
        //*m_backAction.Rot *= q;
        *OutRotation *= q;
    }
}

//void Character::animationMovementControl(D3DXVECTOR3* OutPosition, TAG_ANIM_CHARACTER* OutTag)
void Character::cameraCharacterRotation(const float dt, D3DXQUATERNION* OutRotation, MouseInput* mouseInput)
{

    ICamera* pCurrentCamera = CurrentCamera()();

    static bool tempBool = false;
    if (m_currentStayKey._LAlt)
    {
        if (pCurrentCamera)
        {
            if (pCurrentCamera->GetTagCamera() == TAG_CAMERA::Third_Person || pCurrentCamera->GetTagCamera() == TAG_CAMERA::Default)
            {
                m_rotationForCamera.x += -mouseInput->pitch;
                m_rotationForCamera.y += mouseInput->yaw;
                tempBool = true;
            }
        }
    }
    else // isPressing_LAlt == false
    {
        //JOHN HAN 허리 로테이션 코드 
        //if (m_waistRotation.m_limit)
        //{
        //    D3DXQUATERNION q;
        //    D3DXQuaternionRotationYawPitchRoll(&q, mouseInput->yaw, 0.0f, 0.0f);
        //    *OutRotation *= q;

        //    //D3DXQUATERNION q;
        //    //D3DXQuaternionRotationYawPitchRoll(&q, m_rotationForCamera.y, 0, 0);
        //    //*OutRotation = q;
        //    //m_waistRotation.m_angle = 0;
        //    
        //}
        //else
        //{
        //    m_rotationForCamera.y += mouseInput->yaw;
        //}

        ////이거 지워 -----
        D3DXQUATERNION q;
        D3DXQuaternionRotationYawPitchRoll(&q, mouseInput->yaw, 0.0f, 0.0f);
        *OutRotation *= q;
        //--------

        m_rotationForCamera.x += -mouseInput->pitch;

        // reset rotFotCameraTP
        if (tempBool)
        {
            m_rotationForCamera = Vector3::ZERO;
            tempBool = false;
        }
    }

    //Limiting camera Pitch 
    if (m_rotationForCamera.x < -1.0f)
        m_rotationForCamera.x = -1.0f;
    else if (m_rotationForCamera.x > 1.0f)
        m_rotationForCamera.x = 1.0f;    
}

bool Character::IsMine() const
{
    return m_index == Communication()()->m_myInfo.ID;
}

void Character::applyTarget_Y_Position(OUT D3DXVECTOR3 * pOut)
{  
    HeightMap* pCurrentScene = CurrentScene()()->GetHeightMap();
    if (!pCurrentScene)
        return; //assert(false && "No CurrentScene");

    D3DXVECTOR3 targetPos = *pOut;
    float height = 0;
    bool isIntersected = true;

    if (m_Jump.isJumping)
    {
        targetPos.y += m_Jump.jumpPower - m_Jump.currGravity;
        m_Jump.currGravity += m_Jump.gravity;
        
        isIntersected = pCurrentScene->GetHeight(targetPos, &height);

        if (targetPos.y <= height && m_Jump.jumpPower < m_Jump.currGravity
            /*|| m_isCollidedWithBox&& m_Jump.jumpPower < m_Jump.currGravity*/)
        {            
            //점프 후 착지애니메이션
            //TODO: 높이에 따라서 다른 착지애니메이션
            TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
            if(m_attacking == Attacking::Unarmed)
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Fall_Landing_Additive;
            else if (m_attacking == Attacking::Rifle)
                tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Fall_Landing_Hard;
            Sound()()->Play(TAG_SOUND::FallingGround, Vector3::ZERO, 1.0f, FMOD_2D);

            setAnimation(
                CharacterAnimation::BodyPart::BOTH,
                tagAnim,
                true,
                0.1f,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                setAnimation(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
            
            m_Jump.isJumping = false;
            m_Jump.currGravity = 0.0f;

            ////만약 character발 아래 collider 가 있다면
            //if (m_isCollidedWithBox)
            //    targetPos.y = m_adjust_Y_onCollision;
            //else
                targetPos.y = height;
        }
    }
    else //when no jump
    {
        isIntersected = pCurrentScene->GetHeight(targetPos, &height);
        if (!isIntersected /*|| fabs(height - *pOut.y) > m_Jump.maxStepHeight*/)
        {
            //do nothing
        }
        else
        {
            //만약 character발 아래 collider 가 있다면
            if (m_isCollidedWithBox)
                targetPos.y = m_adjust_Y_onCollision;
            else
                targetPos.y = height;
        }
    }


    *pOut = targetPos;
}

void Character::RifleShooting() //bullet 객체에 대한
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
    {
        BulletPool()()->Fire(Communication()()->m_myInfo, bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::QBZ), ItemInfo::GetBaseDamage(TAG_RES_STATIC::QBZ), TAG_RES_STATIC::QBZ);
        Sound()()->Play(TAG_SOUND::Qbz_NormalShoot,
            GetTransform()->GetPosition(),
            1.0f, FMOD_2D);
        Communication()()->SendEventSound(TAG_SOUND::Qbz_NormalShoot, GetTransform()->GetPosition());

        //총 자체 애니메이션
        m_isNeedRifleAnim = true;
        inven.m_pHand->Set
        (
            TAG_ANIM_WEAPON::Weapon_QBZ_Fire,
            false,
            Item::DEFAULT_BLENDING_TIME,
            Item::DEFAULT_NEXT_WEIGHT,
            Item::DEFAULT_POSITION,
            Item::DEFAULT_FINISH_EVENT_AGO_TIME,
            [this, &inven]() {
            inven.m_pHand->Set(
                TAG_ANIM_WEAPON::Weapon_QBZ_Idle,
                false);
            m_isNeedRifleAnim = false;
        });
    }
    break;

    case TAG_RES_STATIC::Kar98k:
    {
        BulletPool()()->Fire(Communication()()->m_myInfo, bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::Kar98k), ItemInfo::GetBaseDamage(TAG_RES_STATIC::Kar98k), TAG_RES_STATIC::Kar98k);
        Sound()()->Play(TAG_SOUND::Kar98_NormalShoot,
            GetTransform()->GetPosition(),
            1.0f, FMOD_2D);
        Sound()()->addPlay(TAG_SOUND::Kar98_BoltMove0,
            GetTransform()->GetPosition(),
            0.3f, FMOD_2D);
        Sound()()->addPlay(TAG_SOUND::Kar98_BoltMove1,
            GetTransform()->GetPosition(),
            0.8f, FMOD_2D);
        Sound()()->addPlay(TAG_SOUND::Kar98_BoltMove2,
            GetTransform()->GetPosition(),
            1.0f, FMOD_2D);

        Communication()()->SendEventSound(TAG_SOUND::Kar98_NormalShoot, GetTransform()->GetPosition());

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
    D3DXQUATERNION rot = (GetTransform())->GetRotation();
    backAction(&rot);
}
D3DXVECTOR3 Character::FindShootingTargetPos()
{
    auto& inven = m_totalInventory;
    inven.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(inven.m_pHand->GetTagResStatic());

    int numBullet = inven.m_pHand->GetNumBullet();
    inven.m_pHand->SetNumBullet(--numBullet);
    //cout << "총에 남아있는 총알 개수: " << inven.m_pHand->GetNumBullet() << "\n";

    //Goal : get Fire starting position , get fire direction

    //Update in Character::updateTotalInventory()
    //inven.m_pHand->UpdateModel(); //update to get position of frame "gun_bolt" 

    D3DXMATRIX mat
        = inven.m_pHand->GetGunBolt()->CombinedTransformationMatrix  //model space combinde matrix
        * m_framePtr.pHandGun->CombinedTransformationMatrix          // hand gun space matrix
        * GetTransform()->GetTransformationMatrix();                 //character world matrix

    D3DXVECTOR3 bulletFirePos = Matrix::GetTranslation(mat);

    D3DXVECTOR3 bulletDir;
    CurrentCamera()()->CalcPickedPosition(&bulletDir, 1280 / 2, 720 / 2);
    bulletDir = bulletDir - bulletFirePos;
    D3DXVec3Normalize(&bulletDir, &bulletDir);
    //-------------------------

    // ----------------------수정중-------------
    Ray gunRay;
    gunRay.m_pos = bulletFirePos;
    gunRay.m_dir = bulletDir;
    // 나 말고 다른 캐릭터들을 순회하며 충돌여부를 검사한다.

    const auto& others = static_cast<ScenePlay*>(CurrentScene()())->GetOthers();
    for (auto o : others)
    {
        // 먼저 캐릭터의 바운딩스피어와 충돌을 검사한다.
        BoundingSphere bs = o->GetBoundingSphere();

        if (!D3DXSphereBoundProbe(
            &(bs.center + bs.position),
            bs.radius,
            &bulletFirePos,
            &bulletDir)) continue;

        //const float distance = D3DXVec3Length(&(oPos - bulletFirePos));

        //// 탄도학에 의한 예측 낙차 높이를 계산한다.
        //const float varY = Ballistics::GetVarianceY(inven.m_pHand->GetTagResStatic(), distance);
        //D3DXVECTOR3 estimatedDest = bulletDir * distance;
        ////estimatedDest.y += varY;
        //estimatedDest = bulletDir * 10000.0f;
        //
        //// 낙차 높이를 계산한 곳으로 레이를 쏜다.
        //bulletDir = estimatedDest - bulletFirePos;
        //D3DXVec3Normalize(&bulletDir, &bulletDir);

        // 캐릭터의 바운딩박스들과 충돌을 검사한다.
        float minDist = std::numeric_limits<float>::max();
        float dist = std::numeric_limits<float>::max();
        const auto& obbs = o->GetBoundingBoxes();
        for (std::size_t i = 0; i < obbs.size(); i++)
        {
            auto& obb = obbs[i];

            if (Collision::HasCollision(gunRay, obb, &dist))
            {
                // hit
                if (dist < minDist)
                {
                    minDist = dist;
                    m_otherHitPart = i;
                    m_otherHitBox = obb;
                }
            }
        }

        if (minDist != std::numeric_limits<float>::max())
        {
            // 제일 작은 히트한 놈
            cout << "hited part : " << m_otherHitPart << '\n';

            const auto tagWeapon = inven.m_pHand->GetTagResStatic();
            const auto tagPart = static_cast<TAG_COLLIDER_CHARACTER_PART>(m_otherHitPart);
            const float damage
                = ItemInfo::GetBaseDamage(tagWeapon)//Base Weapon Damage
                * ItemInfo::GetDropOffByDistance(minDist, tagWeapon)//Damage drop-off by Distance
                * CharacterInfo::GetHitAreaDamage(tagPart) //Hit Area Damage
                * CharacterInfo::GetWeaponClassDamageByHitZone(tagPart); //Weapon Class Damage By Hit Zone

            o->MinusDamage(damage);
            Communication()()->SendEventMinusDamage(o->GetIndex(), damage);
        }
    }

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

//const BoundingBox& Character::GetBoundingBox()
//{
//    return m_bBox;
//}

std::vector<BoundingBox>& Character::GetBoundingBoxes()
{
    m_boundingBoxes.resize(m_characterParts.size());

    for (std::size_t i = 0; i < m_characterParts.size(); i++)
        m_boundingBoxes[i] = m_characterParts[i]->GetBoundingBoxes().front();

    return IObject::GetBoundingBoxes();
}

void Character::CreateDeathDropBox()
{
    ScenePlay* pScenePlay =
        static_cast<ScenePlay*>(CurrentScene()());

    DeathDropBox* pBox =
        pScenePlay->GetDeathDropBox(m_index);

    const D3DXVECTOR3 pos = GetTransform()->GetPosition();
    pBox->Set(pos, this);

    pScenePlay->InsertObjIntoTotalCellSpace(
        TAG_OBJECT::DeathDropBox,
        pScenePlay->GetCellIndex(pos),
        pBox);
}

void Character::SetReadyAnimation()
{
    auto setAnimation = [this](
        const TAG_ANIM_CHARACTER anim,
        const TAG_ANIM_CHARACTER nextAnim)
    {
        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            anim,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
            [this, nextAnim]()
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                nextAnim, false);
        });
    };

    switch (m_index)
    {
    case 0:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P1_NotReady_Ready, 
                TAG_ANIM_CHARACTER::Lobby_P1_Ready);
        }
        break;
    case 1:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P2_NotReady_Ready,
                TAG_ANIM_CHARACTER::Lobby_P2_Ready);
        }
        break;
    case 2:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P3_NotReady_Ready,
                TAG_ANIM_CHARACTER::Lobby_P3_Ready);
        }
        break;
    case 3:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P4_NotReady_Ready,
                TAG_ANIM_CHARACTER::Lobby_P4_Ready);
        }
        break;
    default:
        {
            assert(false && "Character::SetReadyAnimation(), default case.");
        }
        break;
    }
}

void Character::SetNotReadyAnimation()
{
    auto setAnimation = [this](
        const TAG_ANIM_CHARACTER anim,
        const TAG_ANIM_CHARACTER nextAnim)
    {
        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            anim,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
            [this, nextAnim]()
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                nextAnim, false);
        });
    };

    switch (m_index)
    {
    case 0:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P1_Ready_NotReady,
                TAG_ANIM_CHARACTER::Lobby_P1_NotReady);
        }
        break;
    case 1:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P2_Ready_NotReady,
                TAG_ANIM_CHARACTER::Lobby_P2_NotReady);
        }
        break;
    case 2:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P3_Ready_NotReady,
                TAG_ANIM_CHARACTER::Lobby_P3_NotReady);
        }
        break;
    case 3:
        {
            setAnimation(
                TAG_ANIM_CHARACTER::Lobby_P4_Ready_NotReady,
                TAG_ANIM_CHARACTER::Lobby_P4_NotReady);
        }
        break;
    default:
        {
            assert(false && "Character::SetNotReadyAnimaion(), default case.");
        }
        break;
    }
}

void Character::InitScenePlay()
{
    Transform* pTransform = GetTransform();
    pTransform->SetRotation(OFFSET_ROTATION);

    if (IsMine())
    {
        m_inGameUI.Init(this);
        m_totalInventory.Init(this);
        m_gameOverUI.Init(this);
    }

    if (m_index == 0)
    {
        pTransform->SetPosition(D3DXVECTOR3(7000.0f, 5000.0f, 7000.0f));
    }
    else if (m_index == 1)
    {
        pTransform->SetPosition(D3DXVECTOR3(5000.0f, 5000.0f, 18000.0f));
    }
    else if (m_index == 2)
    {
        pTransform->SetPosition(D3DXVECTOR3(20000.0f, 5000.0f, 6000.0f));
    }
    else if (m_index == 3)
    {
        pTransform->SetPosition(D3DXVECTOR3(19000.0f, 5000.0f, 19000.0f));
    }

    ////new map
    Communication()()->m_roomInfo.playerInfos[0].position = D3DXVECTOR3(7000.0f, 5000.0f, 7000.0f);
    Communication()()->m_roomInfo.playerInfos[0].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[0].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[1].position = D3DXVECTOR3(5000.0f, 5000.0f, 18000.0f);
    Communication()()->m_roomInfo.playerInfos[1].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[1].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[2].position = D3DXVECTOR3(20000.0f, 5000.0f, 6000.0f);
    Communication()()->m_roomInfo.playerInfos[2].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[2].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    Communication()()->m_roomInfo.playerInfos[3].position = D3DXVECTOR3(19000.0f, 5000.0f, 19000.0f);
    Communication()()->m_roomInfo.playerInfos[3].upperAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    Communication()()->m_roomInfo.playerInfos[3].lowerAnimState = static_cast<int>(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    //putting character into TotalCellSpace
    IScene* CS = CurrentScene()();
    m_cellIndex = CS->GetCellIndex(pTransform->GetPosition());                   //캐릭터의 pos에 따라 알맞은 area에 넣어주기
    CS->InsertObjIntoTotalCellSpace(TAG_OBJECT::Character, m_cellIndex, this);   //Object 를 TotalCellSpace(Area)에 넣기
    
    if (IsMine())
    {
        CS->m_NearArea.CreateNearArea(m_cellIndex);                                  //Near Area 계산
    }

    if (IsMine())
    {
        setInfo();
        Camera()()->SetTarget(&m_info);
        m_rotationForCamera = Vector3::ZERO;
        Camera()()->SetCurrentCamera(TAG_CAMERA::Third_Person);
    }

    pAnimation->Set(CharacterAnimation::BodyPart::BOTH, TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);
}

void Character::movementControl(OUT State* OutState)
{
    //점프
    if (m_currentOnceKey._Space && m_stance == Stance::Stand && m_hasChangingState == false)
    {
        m_Jump.isJumping = true;
    }

    float movingFactor = 0.0f;

    //Moving 3개 -----------------------------------------------------
    if(m_moving == Moving::Run)
     {
        if(m_attacking == Attacking::Unarmed)
             movingFactor = MovingFactor::UNARMED_RUN;
        else 
             movingFactor = MovingFactor::RIFLE_RUN;
     }
    else if (m_moving == Moving::Sprint)
    {
        if (m_attacking == Attacking::Unarmed)
             movingFactor = MovingFactor::UNARMED_SPRINT;
        else
             movingFactor = MovingFactor::RIFLE_SPRINT;
    }
    else if (m_moving == Moving::Walk)
    {
        if (m_attacking == Attacking::Unarmed)
            movingFactor = MovingFactor::UNARMED_WALK;
        else
            movingFactor = MovingFactor::RIFLE_WALK;
    }
    
    float dt = Time()()->GetDeltaTime();
    float debugSpeed = DebugMgr()()->HasDebugSpeed() ? m_rootTransform.DEBUG_MOVE_SPEED : 1.0f;
    float dist = movingFactor * m_rootTransform.MOVE_SPEED * dt * debugSpeed;

    //여기
    //점프 직후 이동 금지
    const string& animName = pAnimation->GetLowerAnimationName();
    if (animName == TagAnimation::GetString(TAG_ANIM_CHARACTER::Unarmed_Combat_Fall_Landing_Additive) ||
        animName == TagAnimation::GetString(TAG_ANIM_CHARACTER::Rifle_Combat_Fall_Landing_Hard))
    {
        return;
    }

    //자세 전이 중이면 이동금지
    if (m_isTransitioning)
        return;
    
    //Direction 8개 -----------------------------------------------------
    if (m_currentStayKey._W&&m_currentStayKey._D)
    {
        OutState->position += GetForwardRight() * dist;
    }
    else if (m_currentStayKey._D&&m_currentStayKey._S)
    {
        OutState->position += GetBackwardRight() * dist;
    }
    else if (m_currentStayKey._S&&m_currentStayKey._A)
    {
        OutState->position += GetBackwardLeft() * dist;
    }
    else if (m_currentStayKey._A&&m_currentStayKey._W)
    {
        OutState->position += GetForwardLeft() * dist;
    }
    else if (m_currentStayKey._W)
    {
        OutState->position += GetForward() * dist;
    }
    else if (m_currentStayKey._D)
    {
        OutState->position += GetRight() * dist;
    }
    else if (m_currentStayKey._S)
    {
        OutState->position += GetBackward() * dist;
    }
    else if (m_currentStayKey._A)
    {
        OutState->position += GetLeft() * dist;
    }

    //// RECORD
    //OutState->position = GetTransform()->GetPosition();
}

void Character::animationControl()
{
    //Direction 8개 -----------------------------------------------------
    if (m_currentStayKey._W&&m_currentStayKey._D)
    {
        m_direction = Direction::FrontRight;
    }
    else if (m_currentStayKey._D&&m_currentStayKey._S)
    {
        m_direction = Direction::BackRight;
    }
    else if (m_currentStayKey._S&&m_currentStayKey._A)
    {
        m_direction = Direction::BackLeft;
    }
    else if (m_currentStayKey._A&&m_currentStayKey._W)
    {
        m_direction = Direction::FrontLeft;
    }
    else if (m_currentStayKey._W)
    {
        m_direction = Direction::Front;

    }
    else if (m_currentStayKey._D)
    {
        m_direction = Direction::Right;
    }
    else if (m_currentStayKey._S)
    {
        m_direction = Direction::Back;
    }
    else if (m_currentStayKey._A)
    {
        m_direction = Direction::Left;
    }
    else
    {
        m_direction = Direction::StandStill;
    }

    //Moving 3개 -----------------------------------------------------
    if (m_currentStayKey._LShift && !m_currentStayKey._LCtrl && m_stance != Stance::Prone)
    {
        switch (m_direction)
        {
        case Direction::Back:
        case Direction::BackLeft:
        case Direction::BackRight:
        case Direction::Left:
        case Direction::Right:
            m_moving = Moving::Run;
            break;

        default:
            m_moving = Moving::Sprint;
            break;
        }
    }
    else if (m_currentStayKey._LCtrl && !m_currentStayKey._LShift)
    {
        m_moving = Moving::Walk;
    }
    else
    {
        m_moving = Moving::Run;
    }

    m_lowerAnimState = AnimationState::Get(m_attacking, m_stance, m_moving, m_direction);
}

void Character::setInfo()
{
    m_info.pTransform = GetTransform();
    m_info.pRotationForCamera = &m_rotationForCamera;
    m_info.pHead = m_framePtr.pHead;
    m_info.pTPP = m_framePtr.pTPP;
    m_info.pFPP = m_framePtr.pFPP;
    m_info.pHandGun = m_framePtr.pHandGun;
}

void Character::MinusDamage(const float damage)
{
    m_health -= damage;
    if (m_health <= 0.0f)
    {
        m_health = 0.0f;
    }
}

int Character::GetKillNum() const
{
    return m_killNum;
}

void Character::SetKillNum(const int killNum)
{
    m_killNum = killNum;
}

bool Character::GetIsKill() const
{
    return m_isKill;
}

void Character::SetIsKill(const bool isKill)
{
    m_isKill = isKill;
}

bool Character::GetIsEatEquip() const
{
    return m_isEatEquip;
}

void Character::SetIsEatEquip(const bool isEatEquip)
{
    m_isEatEquip = isEatEquip;
}

bool Character::GetIsGameOver() const
{
    return m_isGameOver;
}

void Character::SetIsGameOver(const bool isGameOver)
{
    m_isGameOver = isGameOver;
}

Character::InGameUI& Character::GetInGameUI()
{
    return m_inGameUI;
}

Character::TotalInventory& Character::GetTotalInventory()
{
    return m_totalInventory;
}

Character::WaitBackAction& Character::GetWaitBackAction()
{
    return m_backAction;
}

void Character::MoveItemFieldToHead(Item* pItem)
{
    TotalInventory& inven = m_totalInventory;

    inven.m_pEquipHead = pItem;
    pItem->SetIsInInventory(true);
    pItem->SetIsRenderEffectMesh(false);
    pItem->SetIsRenderSkinnedMesh(true);
}

void Character::MoveItemFieldToArmor(Item* pItem)
{
    TotalInventory& inven = m_totalInventory;

    inven.m_pEquipArmor = pItem;
    pItem->SetIsInInventory(true);
    pItem->SetIsRenderEffectMesh(false);
    pItem->SetIsRenderSkinnedMesh(true);
}

void Character::MoveItemFieldToBack(Item* pItem)
{
    TotalInventory& inven = m_totalInventory;

    inven.m_pEquipBack = pItem;
    pItem->SetIsInInventory(true);
    pItem->SetIsRenderEffectMesh(false);
    pItem->SetIsRenderSkinnedMesh(true);
}

void Character::MoveItemFieldToPrimary(Item* pItem)
{
    TotalInventory& inven = m_totalInventory;

    inven.m_pWeaponPrimary = pItem;
    pItem->SetIsInInventory(true);
    pItem->SetIsRenderEffectMesh(false);
    pItem->SetIsRenderSkinnedMesh(true);
}

void Character::MoveItemFieldToSecondary(Item* pItem)
{
    TotalInventory& inven = m_totalInventory;

    inven.m_pWeaponSecondary = pItem;
    pItem->SetIsInInventory(true);
    pItem->SetIsRenderEffectMesh(false);
    pItem->SetIsRenderSkinnedMesh(true);
}

void Character::MoveItemFieldToInventory(Item* pItem)
{
    TotalInventory& inven = m_totalInventory;

    inven.m_mapInventory[pItem->GetTagResStatic()].emplace_back(pItem);
    pItem->SetIsInInventory(true);
    pItem->SetIsRenderEffectMesh(false);
}

void Character::MoveItemHeadToField()
{
}

void Character::MoveItemArmorToField()
{
}

void Character::MoveItemBackToField()
{
}

void Character::MoveItemPrimaryToField()
{
}

void Character::MoveItemSecondaryToField()
{
}

void Character::MoveItemPrimaryToHand()
{
    TotalInventory& ti = m_totalInventory;
    ti.m_pHand = ti.m_pWeaponPrimary;
    ti.m_pWeaponPrimary = nullptr;
}

void Character::MoveItemSecondaryToHand()
{
    TotalInventory& ti = m_totalInventory;
    ti.m_pHand = ti.m_pWeaponSecondary;
    ti.m_pWeaponSecondary = nullptr;
}

void Character::MoveItemHandToPrimary()
{
    TotalInventory& ti = m_totalInventory;
    ti.m_pWeaponPrimary = ti.m_pHand;
    ti.m_pHand = nullptr;
}

void Character::MoveItemHandToSecondary()
{
    TotalInventory& ti = m_totalInventory;
    ti.m_pWeaponSecondary = ti.m_pHand;
    ti.m_pHand = nullptr;
}

D3DXVECTOR3 Character::GetUp()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::UP, rot);
    //dir.y *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetForward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetBackward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD, rot);
    dir.z *= -1.0f; 
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::LEFT, rot);
    dir.x *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::RIGHT, rot);
    dir.x *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetForwardLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD_RIGNT, rot);//왜 forward_Right인지..?? 이래야 잘 돌아 가는 이유는?
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetForwardRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD_LEFT, rot); //왜 forward_left인지..?? 이래야 잘 돌아 가는 이유는?
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetBackwardRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD_LEFT, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::GetBackwardLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD_RIGHT, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

void Character::updateBone()
{
    //*****여기 변경되면 반드시 Item::updateBone()도 변경시켜주길 바람!!!
    // modify local bones

    // head, clavles
    D3DXMATRIX rHead;

    //머리 Rotaion
    D3DXMatrixRotationY(&rHead, m_headRotation.m_angle);
    m_framePtr.pHead->TransformationMatrix *= rHead;

    if (m_totalInventory.m_pHand)//총을 들었을때 손 Rotation
    {
        m_framePtr.pLeftClavicle->TransformationMatrix *= rHead;    //왼쪽 손
        m_framePtr.pRightClavicle->TransformationMatrix *= rHead;   //오른쪽 손
    }
    
    //허리 rotation 돌리는것
    D3DXMATRIX rWaist;
    D3DXMatrixRotationX(&rWaist, m_waistRotation.m_angle);
    m_framePtr.pWaist->TransformationMatrix *= rWaist;

    // RECORD
    // for root motion animation
    m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
}

void Character::communicate()
{
}

void Character::rotateWaist(const float quantity)
{
    auto& wr = m_waistRotation;
    wr.m_limit = false;

    wr.m_angle += quantity/** m_waistRotation.QUANTITY_FACTOR*/;

    if (wr.m_angle < -wr.LIMIT_OF_ANGLE)
    {
        wr.m_angle = -wr.LIMIT_OF_ANGLE;
        wr.m_limit = true;
        //wr.m_angle = 0;
    }
        
    else if (wr.m_angle > wr.LIMIT_OF_ANGLE)
    {
        wr.m_angle = wr.LIMIT_OF_ANGLE;
        wr.m_limit = true;
        //wr.m_angle = 0;
    }
        
}

void Character::rotateHead(const float quantity)
{
    auto& hr = m_headRotation;

    hr.m_angle += quantity;

    if (hr.m_angle < -hr.LIMIT_OF_ANGLE)
        hr.m_angle = -hr.LIMIT_OF_ANGLE;
    else if (hr.m_angle > hr.LIMIT_OF_ANGLE)
        hr.m_angle = hr.LIMIT_OF_ANGLE;
}


int Character::GetIndex() const
{
    return m_index;
}

string Character::GetNickName() const
{
    return Communication()()->m_roomInfo.playerInfos[m_index].nickname;
}

float Character::GetCharacterHealth() const
{
    return m_health;
}

bool Character::GetCharacterIsDead() const
{
    return m_isDead;
}

const BoundingBox& Character::GetBoundingBox()
{
    return m_bBox;
}

void Character::ResetIsDamaged()
{
    m_isDamaged = false;
}

bool Character::IsDamaged() const
{
    return m_isDamaged;
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

CharacterAnimation* Character::GetCharacterAnimation()
{
    return pAnimation;
}

void Character::AddPart(CharacterPart* pPart)
{
    m_characterParts[static_cast<std::size_t>(pPart->GetTagColliderCharacterPart())] = pPart;
}

D3DXVECTOR3 Character::GetWaistPosition()
{
    return Matrix::GetTranslation(m_framePtr.pWaist->CombinedTransformationMatrix * GetTransform()->GetTransformationMatrix());
}

bool Character::IsFire() const
{
    return m_isFire;
}
