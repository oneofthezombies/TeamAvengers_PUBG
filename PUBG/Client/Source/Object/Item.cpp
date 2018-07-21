#include "stdafx.h"
#include "Item.h"
#include "ItemInfo.h"
#include "EffectMeshRenderer.h"
#include "SkinnedMeshController.h"
#include "Light.h"
#include "UIImage.h"
#include "UIText.h"
#include "ResourceInfo.h"
#include "DeathDropBox.h"

using BodyPart = CharacterAnimation::BodyPart;

const float Item::DEFAULT_BLENDING_TIME = 0.3f;
const float Item::DEFAULT_NEXT_WEIGHT = 0.0f;
const float Item::DEFAULT_POSITION = 0.0f;
const float Item::DEFAULT_FINISH_EVENT_AGO_TIME = 0.0f;

Item::Item(
    const TAG_RES_STATIC tag,
    const std::string& name,
    const D3DXVECTOR3&   position,
    const D3DXVECTOR3&   rotation,
    const D3DXVECTOR3&   scale)
    : IObject(TAG_OBJECT::Item)

    , m_tagResStatic(tag)
    , m_name(name)

    , m_durability(0.0f)
    , m_count(0)
    , m_numBullet(0)
    , m_auto(false)
    
    , pEffectMeshRenderer(nullptr)
    , m_isRenderEffectMesh(false)

    , pSkinnedMeshController(nullptr)
    , m_isRenderSkinnedMesh(false)

    , m_pUIImage(nullptr)
    , m_pUIImage2(nullptr)
    , pUIText(nullptr)
    , m_inInventory(false)

    , pGunBolt(nullptr)

    , m_pFramePtr(nullptr)
    , pDeathDropBox(nullptr)

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
    SAFE_DELETE(m_pUIImage);
    SAFE_DELETE(m_pUIImage2);
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

        D3DXVec3TransformCoord(
            &center, 
            &pEM->m_boundingSphere.center, 
            &GetTransform()->GetTransformationMatrix());

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
    case TAG_ITEM_CATEGORY::Attach:
    //부착물도 setting용이 따로있음!!
        
    case TAG_ITEM_CATEGORY::Ammo:
    case TAG_ITEM_CATEGORY::Consumable:
        {
            const auto pathName = ResourceInfo::GetUIPathFileName(tag);
            m_pUIImage = new UIImage(pathName.first, pathName.second, Vector3::ZERO, this, nullptr);
            m_pUIImage->SetIsRender(false);
            pUIText = new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
                D3DXVECTOR2(100.0f, 20.0f),
                "",
                D3DCOLOR_XRGB(255, 255, 255),
                m_pUIImage);
            pUIText->SetPosition(D3DXVECTOR3(50.0f, 10.0f, 0.0f));
        }
        break;

    case TAG_ITEM_CATEGORY::Armor:
        {
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathFileName = ResourceInfo::GetPathFileName(TAG_RES_ANIM_EQUIPMENT::Armor_Lv1_Anim);

            std::size_t skinnedMeshIndex;
            const bool available = 
                Resource()()->GetAvailableIndexForSkinnedMesh(
                    pathFileName.first, 
                    pathFileName.second, 
                    &skinnedMeshIndex);

            assert(
                available && 
                "Item::setup(), skinned mesh is not available.");

            pSkinnedMeshController->SetSkinnedMesh(
                Resource()()->GetSkinnedMesh(
                    pathFileName.first, 
                    pathFileName.second, 
                    skinnedMeshIndex));

            Resource()()->AddSkinnedMeshCount(
                pathFileName.first, 
                pathFileName.second);

            m_pFramePtr = new FramePtr;
            setFramePtr();
            addAnimationBackupFrameForEquip();

            Set(BodyPart::BOTH, TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

            const auto pathName2 = ResourceInfo::GetUIPathFileName(tag);
            m_pUIImage = new UIImage(pathName2.first, pathName2.second, Vector3::ZERO, this, nullptr);
            m_pUIImage->SetIsRender(false);
            pUIText = new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
                D3DXVECTOR2(100.0f, 20.0f),
                "",
                D3DCOLOR_XRGB(255, 255, 255),
                m_pUIImage);
            pUIText->SetPosition(D3DXVECTOR3(50.0f, 10.0f, 0.0f));
        }
        break;
    case TAG_ITEM_CATEGORY::Back:
        {
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathFileName = ResourceInfo::GetPathFileName(TAG_RES_ANIM_EQUIPMENT::Back_Lv1_Anim);

            std::size_t skinnedMeshIndex;
            const bool available =
                Resource()()->GetAvailableIndexForSkinnedMesh(
                    pathFileName.first,
                    pathFileName.second,
                    &skinnedMeshIndex);

            assert(
                available &&
                "Item::setup(), skinned mesh is not available.");

            pSkinnedMeshController->SetSkinnedMesh(
                Resource()()->GetSkinnedMesh(
                    pathFileName.first,
                    pathFileName.second,
                    skinnedMeshIndex));

            Resource()()->AddSkinnedMeshCount(
                pathFileName.first,
                pathFileName.second);

            m_pFramePtr = new FramePtr;
            setFramePtr();
            addAnimationBackupFrameForEquip();

            Set(BodyPart::BOTH, TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

            const auto pathName2 = ResourceInfo::GetUIPathFileName(tag);
            m_pUIImage = new UIImage(pathName2.first, pathName2.second, Vector3::ZERO, this, nullptr);
            m_pUIImage->SetIsRender(false);
            pUIText = new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
                D3DXVECTOR2(100.0f, 20.0f),
                "",
                D3DCOLOR_XRGB(255, 255, 255),
                m_pUIImage);
            pUIText->SetPosition(D3DXVECTOR3(50.0f, 10.0f, 0.0f));
        }
        break;

    case TAG_ITEM_CATEGORY::Head:
        {
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathFileName = ResourceInfo::GetPathFileName(TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim);

            std::size_t skinnedMeshIndex;
            const bool available =
                Resource()()->GetAvailableIndexForSkinnedMesh(
                    pathFileName.first,
                    pathFileName.second,
                    &skinnedMeshIndex);

            assert(
                available &&
                "Item::setup(), skinned mesh is not available.");

            pSkinnedMeshController->SetSkinnedMesh(
                Resource()()->GetSkinnedMesh(
                    pathFileName.first,
                    pathFileName.second,
                    skinnedMeshIndex));

            Resource()()->AddSkinnedMeshCount(
                pathFileName.first,
                pathFileName.second);

            m_pFramePtr = new FramePtr;
            setFramePtr();
            addAnimationBackupFrameForEquip();

            Set(BodyPart::BOTH, TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

            const auto pathName2 = ResourceInfo::GetUIPathFileName(tag);
            m_pUIImage = new UIImage(pathName2.first, pathName2.second, Vector3::ZERO, this, nullptr);
            m_pUIImage->SetIsRender(false);
            pUIText = new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
                D3DXVECTOR2(100.0f, 20.0f),
                "",
                D3DCOLOR_XRGB(255, 255, 255),
                m_pUIImage);
            pUIText->SetPosition(D3DXVECTOR3(50.0f, 10.0f, 0.0f));
        }
        break;
    
    case TAG_ITEM_CATEGORY::Rifle:
        {
            // TODO : putin or equip으로 옮겨야 함
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathFileName = 
                ResourceInfo::GetPathFileName(
                    ResourceInfo::GetTagResAnimWeapon(tag));

            std::size_t skinnedMeshIndex;
            const bool available =
                Resource()()->GetAvailableIndexForSkinnedMesh(
                    pathFileName.first,
                    pathFileName.second,
                    &skinnedMeshIndex);

            assert(
                available &&
                "Item::setup(), skinned mesh is not available.");

            pSkinnedMeshController->SetSkinnedMesh(
                Resource()()->GetSkinnedMesh(
                    pathFileName.first,
                    pathFileName.second,
                    skinnedMeshIndex));

            Resource()()->AddSkinnedMeshCount(
                pathFileName.first,
                pathFileName.second);

            //총알이 나갈 위치
            if (m_tagResStatic == TAG_RES_STATIC::QBZ)
            {
                pGunBolt = pSkinnedMeshController->FindFrame("gun_bolt");
                m_pUIImage2 = new UIImage("./Resource/UI/Inventory/Weapon/", "QBZ.png", Vector3::ZERO, this, nullptr);
            }
            else if (m_tagResStatic == TAG_RES_STATIC::Kar98k)
            {
                pGunBolt = pSkinnedMeshController->FindFrame("Gun_bolt_02");

                m_pUIImage2 = new UIImage("./Resource/UI/Inventory/Weapon/", "Kar98k.png", Vector3::ZERO, this, nullptr);
                //가장 처음은 idle로 셋해줌
                Set(TAG_ANIM_WEAPON::Weapon_Kar98k_Idle);
                UpdateAnimation();
                UpdateModel();
            }
            const auto pathName2 = ResourceInfo::GetUIPathFileName(tag);
            m_pUIImage = new UIImage(pathName2.first, pathName2.second, Vector3::ZERO, this, nullptr);
            m_pUIImage->SetIsRender(false);
            pUIText = new UIText(Resource()()->GetFont(TAG_FONT::Inventory_Ground),
                D3DXVECTOR2(100.0f, 20.0f),
                "",
                D3DCOLOR_XRGB(255, 255, 255),
                m_pUIImage);
            pUIText->SetPosition(D3DXVECTOR3(50.0f, 10.0f, 0.0f));
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

void Item::SetIsRenderUIImage(const bool isRenderUIImage)
{
    assert(m_pUIImage && "Item::SetInRenderUIImage(), ui image is null.");

    m_pUIImage->SetIsRender(isRenderUIImage);
    //pUIText->Render
}

bool Item::IsRenderUIImage() const
{
    assert(m_pUIImage && "Item::IsRenderUIImage(), ui image is null.");

    return m_pUIImage->IsRender();
}

void Item::SetPosition(const D3DXVECTOR3& position)
{
    GetTransform()->SetPosition(position);
}

void Item::SetUIPosition(const D3DXVECTOR2& position)
{
    assert(m_pUIImage && "Item::SetUIPosition(), ui image is null.");

    m_pUIImage->SetPosition(D3DXVECTOR3(position.x, position.y, 0.0f));
}

const std::string& Item::GetName()
{
    return m_name;
}

UIText* Item::GetUIText()
{
    return pUIText;
}

UIImage* Item::GetUIImage()
{
    return m_pUIImage;
}

UIImage * Item::GetUIImage2()
{
    return m_pUIImage2;
}

void Item::SetIsInInventory(bool state)
{
    m_inInventory = state;
}

bool Item::IsInInventory()
{
    return m_inInventory;
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
    //if (this != NULL)
    //    return m_auto;

    return m_auto;
}

Frame* Item::GetGunBolt() const
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

SkinnedMesh* Item::GetSkinnedMesh() const
{
    return pSkinnedMeshController->GetSkinnedMesh();
}

bool Item::IsInDeathDropBox() const
{
    return pDeathDropBox ? true : false;
}

void Item::DeleteThisInDeathDropBox()
{
    pDeathDropBox->DeleteThisItem(this);
    this->pDeathDropBox = nullptr;
}

void Item::SetDeathDropBox(DeathDropBox* pDeathDropBox)
{
    this->pDeathDropBox = pDeathDropBox;
}

int Item::GetDeathDropBoxIndex() const
{
    assert(pDeathDropBox && "Item::GetDeathDropBoxIndex()");

    return pDeathDropBox->GetIndex();
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

void Item::OnKar98kReload(const int numReload)
{
    if (numReload == 1)
    {
        OnKar98kReloadEnd();
    }
    else
    {
        Sound()()->addPlay(
            TAG_SOUND::Kar98_Reload1, 
            GetTransform()->GetPosition(), 
            0.0f,
            FMOD_2D);

        Set(
            TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Loop,
            false,
            Item::DEFAULT_BLENDING_TIME,
            Item::DEFAULT_NEXT_WEIGHT,
            Item::DEFAULT_POSITION,
            Item::DEFAULT_FINISH_EVENT_AGO_TIME,
            std::bind(&Item::OnKar98kReload, this, numReload - 1)
        );
    }
}

void Item::OnKar98kReloadEnd()
{
    Sound()()->addPlay(
        TAG_SOUND::Kar98_Reload2, 
        GetTransform()->GetPosition(), 
        0.0f,
        FMOD_2D);
    Sound()()->addPlay(TAG_SOUND::Kar98_Reload3, 
        GetTransform()->GetPosition(), 
        0.4f,
        FMOD_2D);

    Set(
        TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_End,
        false,
        Item::DEFAULT_BLENDING_TIME,
        Item::DEFAULT_NEXT_WEIGHT,
        Item::DEFAULT_POSITION,
        Item::DEFAULT_FINISH_EVENT_AGO_TIME,
        [this]()
    {
        Set(
            TAG_ANIM_WEAPON::Weapon_Kar98k_Idle,
            false
        );
    });
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

void Item::SetTrackPosition(const std::size_t index, const double position)
{
    assert(
        pSkinnedMeshController &&
        "Item::SetPosition(), skinned mesh controller is null.");

    pSkinnedMeshController->SetTrackPosition(index, position);
}

void Item::SetSubTrackPosition(const std::size_t index, const double position)
{
    assert(
        pSkinnedMeshController &&
        "Item::SetSubPosition(), skinned mesh controller is null.");

    pSkinnedMeshController->SetSubTrackPosition(index, position);
}
