#include "stdafx.h"
#include "Item.h"
#include "ItemInfo.h"
#include "EffectMeshRenderer.h"
#include "SkinnedMeshController.h"
#include "DirectionalLight.h"
#include "UIImage.h"
#include "ResourceInfo.h"

using BodyPart = CharacterAnimation::BodyPart;

const float Item::DEFAULT_BLENDING_TIME = 0.3f;
const float Item::DEFAULT_NEXT_WEIGHT = 0.0f;
const float Item::DEFAULT_POSITION = 0.0f;
const float Item::DEFAULT_FINISH_EVENT_AGO_TIME = 0.0f;

Item::Item(
    const TAG_RES_STATIC tag,
    const D3DXVECTOR3&   position,
    const D3DXVECTOR3&   rotation,
    const D3DXVECTOR3&   scale)
    : IObject(TAG_OBJECT::Item)

    , m_tagResStatic(tag)
    , m_durability(0.0f)
    , m_count(0)
    , m_numBullet(0)
    , m_auto(false)
    
    , pEffectMeshRenderer(nullptr)
    , m_isRenderEffectMesh(false)

    , pSkinnedMeshController(nullptr)
    , m_isRenderSkinnedMesh(false)

    , pUIImage(nullptr)

    , pGunBolt(nullptr)

    , m_pFramePtr(nullptr)

{
    Transform* pTr = GetTransform();
    pTr->SetPosition(position);
    pTr->SetRotation(rotation);
    pTr->SetScale(scale);
    pTr->Update();

    setup(tag);

    m_boundingSphere = pEffectMeshRenderer->GetBoundingSphere();
}

Item::~Item()
{
    SAFE_DELETE(m_pFramePtr);
}

void Item::OnUpdate()
{
}

void Item::OnRender()
{
    if (m_isRenderEffectMesh)
    {
        EffectMesh* pEM = pEffectMeshRenderer->GetEffectMesh();
        D3DXVECTOR3 center = Vector3::ZERO;
        D3DXVec3TransformCoord(&center, &pEM->m_boundingSphere.center, &GetTransform()->GetTransformationMatrix());
        if(CurrentCamera()()->IsObjectInsideFrustum(center, pEM->m_boundingSphere.radius))
            pEffectMeshRenderer->Render(bind(&Item::setGlobalVariable, this, _1));
    }
        
    if (m_isRenderSkinnedMesh)
    {
        pSkinnedMeshController->Render(
            GetTransform()->GetTransformationMatrix(), 
            std::bind(&Item::setGlobalVariable, 
            this, _1));
    }

    m_boundingSphere.position = GetTransform()->GetPosition();
    m_boundingSphere.Render();
}

void Item::setup(const TAG_RES_STATIC tag)
{
    //내구성 셋팅
    m_durability = ItemInfo::GetDefaultDurability(tag);

    //아이템 개수 셋팅
    switch (tag)
    {
    case TAG_RES_STATIC::Ammo_5_56mm:
    case TAG_RES_STATIC::Ammo_7_62mm:
        m_count = Item::DEFAULT_NUM_BULLET;
        break;

    case TAG_RES_STATIC::Bandage:
        m_count = Item::DEFAULT_NUM_BANDAGE;
        break;

    default: 
        m_count = 1; 
        break;
    }

    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    m_isRenderEffectMesh = true;
    pEffectMeshRenderer->SetEffectMesh(tag);

    switch (ItemInfo::GetItemCategory(tag))
    {
    case TAG_ITEM_CATEGORY::Ammo:
    case TAG_ITEM_CATEGORY::Attach:
    case TAG_ITEM_CATEGORY::Consumable:
        {
            const auto pathName = ResourceInfo::GetUIPathFileName(tag);
            pUIImage = new UIImage(pathName.first, pathName.second, Vector3::ZERO, this, nullptr);
            pUIImage->SetIsRender(false);
            D3DXMATRIX s;
            //아이콘 이미지 size 조절
            D3DXMatrixScaling(&s, 0.2f, 0.2f, 0.0f);
            pUIImage->SetTransform(s);
        }
        break;

    case TAG_ITEM_CATEGORY::Armor:
        {
    
    
        }
        break;
    case TAG_ITEM_CATEGORY::Back:
        {
    
        }
        break;
    case TAG_ITEM_CATEGORY::Head:
        {
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathFileName = ResourceInfo::GetPathFileName(TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim);
            pSkinnedMeshController->SetSkinnedMesh(Resource()()->GetSkinnedMesh(pathFileName.first, pathFileName.second));
            m_pFramePtr = new FramePtr;
            setFramePtr();
            addAnimationBackupFrameForEquip();

            Set(BodyPart::BOTH, TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

            const auto UIpathName = ResourceInfo::GetUIPathFileName(tag);
            pUIImage = new UIImage(UIpathName.first, UIpathName.second, Vector3::ZERO, this, nullptr);
        }
        break;
    
    case TAG_ITEM_CATEGORY::Rifle:
        {
            // TODO : putin or equip으로 옮겨야 함
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathName = ResourceInfo::GetPathFileName(ResourceInfo::GetTagResAnimWeapon(tag));
            pSkinnedMeshController->SetSkinnedMesh(Resource()()->GetSkinnedMesh(pathName.first, pathName.second));

            //총알이 나갈 위치
            if (m_tagResStatic == TAG_RES_STATIC::QBZ)
            {
                pGunBolt = pSkinnedMeshController->FindFrame("gun_bolt");
            }
            else if (m_tagResStatic == TAG_RES_STATIC::Kar98k)
            {
                pGunBolt = pSkinnedMeshController->FindFrame("Gun_bolt_02");

                //가장 처음은 idle로 셋해줌
                Set(TAG_ANIM_WEAPON::Weapon_Kar98k_Idle);
                UpdateAnimation();
                UpdateModel();
            }
            assert(pGunBolt && "Item::setup(), pGunBolt is null.");
        }
        break;
    
    //case TAG_ITEM_CATEGORY::Melee:
    //case TAG_ITEM_CATEGORY::Throwable:
    default:
        {
            assert(false && "Item::setup(), item category default case.");
        }
        break;
    }
}

void Item::setGlobalVariable(LPD3DXEFFECT pEffect)
{
    pEffect->SetMatrix(
        Shader::World,
        &GetTransform()->GetTransformationMatrix());

    DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
    D3DXVECTOR3 lightDir = light->GetDirection();
    pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
}

void Item::addAnimationBackupFrameForEquip()
{
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("pelvis"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_twist_01_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("foot_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_twist_01_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ball_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_twist_01_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("foot_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_twist_01_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ball_l"));
}

void Item::setFramePtr()
{
    m_pFramePtr->pWaist         = pSkinnedMeshController->FindFrame("spine_01");
    m_pFramePtr->pRoot          = pSkinnedMeshController->FindFrame("root");
    m_pFramePtr->pHead          = pSkinnedMeshController->FindFrame("head");
    m_pFramePtr->pLeftClavicle  = pSkinnedMeshController->FindFrame("clavicle_l");
    m_pFramePtr->pRightClavicle = pSkinnedMeshController->FindFrame("clavicle_r");

    m_pFramePtr->pHandGun       = pSkinnedMeshController->FindFrame("item_r");

    m_pFramePtr->pTPP           = pSkinnedMeshController->FindFrame("camera_tpp");
    m_pFramePtr->pFPP           = pSkinnedMeshController->FindFrame("camera_fpp");

    FramePtr*& p = m_pFramePtr;
    assert(
        p->pWaist &&
        p->pRoot &&
        p->pHead &&
        p->pLeftClavicle &&
        p->pRightClavicle &&
        p->pHandGun &&
        p->pTPP &&
        p->pFPP &&
        "Item::setFramePtr(), pointer is null.");
}

TAG_RES_STATIC Item::GetTagResStatic() const
{
    return m_tagResStatic;
}

void Item::SetDurability(const float durability)
{
    m_durability = durability;
}

float Item::GetDurability() const
{
    return m_durability;
}

void Item::SetCount(const int count)
{
    m_count = count;
}

int Item::GetCount() const
{
    return m_count;
}

void Item::SetIsRenderEffectMesh(const bool isRenderEffectMesh)
{
    m_isRenderEffectMesh = isRenderEffectMesh;
}

bool Item::IsRenderEffectMesh() const
{
    return m_isRenderEffectMesh;
}

void Item::SetIsRenderSkinnedMesh(const bool isRenderSkinnedMesh)
{
    m_isRenderSkinnedMesh = isRenderSkinnedMesh;
}

bool Item::IsRenderSkinnedMesh() const
{
    return m_isRenderSkinnedMesh;
}

void Item::SetInRenderUIImage(const bool isRenderUIImage)
{
    assert(pUIImage && "Item::SetInRenderUIImage(), ui image is null.");

    pUIImage->SetIsRender(isRenderUIImage);
}

bool Item::IsRenderUIImage() const
{
    assert(pUIImage && "Item::IsRenderUIImage(), ui image is null.");

    return pUIImage->IsRender();
}

void Item::SetPosition(const D3DXVECTOR3& position)
{
    GetTransform()->SetPosition(position);
}

void Item::SetUIPosition(const D3DXVECTOR2& position)
{
    assert(pUIImage && "Item::SetUIPosition(), ui image is null.");

    pUIImage->SetPosition(D3DXVECTOR3(position.x, position.y, 0.0f));
}

void Item::SetNumBullet(const int numBullet)
{
    //TODO: 장탄수를 넘어서면 안된다
    m_numBullet = numBullet;
}

int Item::GetNumBullet() const
{
    return m_numBullet;
}

void Item::ChangeAuto()
{
    m_auto = !m_auto;
}

bool Item::GetAuto()
{
    return m_auto;
}

Frame * Item::GetGunBolt() const
{
    return pGunBolt;
}

void Item::UpdateAnimation()
{
    if (pSkinnedMeshController)
    {
        pSkinnedMeshController->UpdateAnimation();
    }
}

void Item::UpdateModel()
{
    if (pSkinnedMeshController)
    {
        pSkinnedMeshController->UpdateModel();
    }
}

void Item::UpdateBone(Item* pHand, const float headRot, const float waistRot)
{
    //Character::updateBone()가 수정되면 이곳도 같도록 변경해야함
    D3DXMATRIX rHead;

    D3DXMatrixRotationY(&rHead, headRot);
    m_pFramePtr->pHead->TransformationMatrix *= rHead;

    if (pHand)
    {
        m_pFramePtr->pLeftClavicle->TransformationMatrix *= rHead;
        m_pFramePtr->pRightClavicle->TransformationMatrix *= rHead;
    }

    D3DXMATRIX rWaist;
    D3DXMatrixRotationX(&rWaist, waistRot);
    m_pFramePtr->pWaist->TransformationMatrix *= rWaist;

    m_pFramePtr->pRoot->TransformationMatrix = Matrix::IDENTITY;
}

//for 아이템 자체 애니메이션
void Item::Set(
    const TAG_ANIM_WEAPON tag,
    const bool isBlend, 
    const float blendingTime,
    const float nextWeight, 
    const float position)
{
    pSkinnedMeshController->SetAnimation(
        false,
        TagAnimation::GetString(tag),
        TagAnimation::GetSpeed(tag),
        isBlend,
        blendingTime,
        nextWeight,
        position);
}

void Item::Set(
    const TAG_ANIM_WEAPON tag,
    const bool isBlend,
    const float blendingTime,
    const float nextWeight,
    const float position,
    const float finishEventAgoTime,
    const std::function<void()>& finishEvent)
{
    pSkinnedMeshController->SetAnimation(
        false,
        TagAnimation::GetString(tag),
        TagAnimation::GetSpeed(tag),
        isBlend,
        blendingTime,
        nextWeight,
        position,
        finishEventAgoTime,
        finishEvent);
}

void Item::Set(
    const TAG_ANIM_WEAPON tag, 
    const bool isBlend, 
    const float blendingTime, 
    const float nextWeight, 
    const float position, 
    const float loopEventPeriod,
    const std::function<void()>& loopEvent,
    const float finishEventAgoTime, 
    const std::function<void()>& finishEvent)
{
    pSkinnedMeshController->SetAnimation(
        false,
        TagAnimation::GetString(tag),
        TagAnimation::GetSpeed(tag),
        isBlend,
        blendingTime,
        nextWeight,
        position,
        loopEventPeriod,
        loopEvent,
        finishEventAgoTime,
        finishEvent);
}

bool Item::HasFinishEvent() const
{
    return pSkinnedMeshController->HasFinishEvent();
}

//for 장비 애니메이션
void Item::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float blendingTime,
    const float nextWeight,
    const float position)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position);
    }
}

void Item::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float blendingTime,
    const float nextWeight,
    const float position,
    const float finishEventAgoTime,
    const std::function<void()>& finishEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }
}

void Item::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float blendingTime,
    const float nextWeight,
    const float position,
    const float loopEventPeriod,
    const std::function<void()>& loopEvent,
    const float finishEventAgoTime,
    const std::function<void()>& finishEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);
    }
}