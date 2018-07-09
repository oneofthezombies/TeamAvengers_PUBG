#include "stdafx.h"
#include "Item.h"
#include "ItemInfo.h"
#include "EffectMeshRenderer.h"
#include "SkinnedMeshController.h"
#include "DirectionalLight.h"
#include "UIImage.h"
#include "ResourceInfo.h"

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

    , pEffectMeshRenderer(nullptr)
    , m_isRenderEffectMesh(false)

    , pSkinnedMeshController(nullptr)
    , m_isRenderSkinnedMesh(false)

    , pUIImage(nullptr)

    , pGunBolt(nullptr)

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
        pSkinnedMeshController->Render(GetTransform()->GetTransformationMatrix(), std::bind(&Item::setGlobalVariable, this, _1));

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
        }
        break;

    case TAG_ITEM_CATEGORY::Armor:
    case TAG_ITEM_CATEGORY::Back:
    case TAG_ITEM_CATEGORY::Head:
        {
            pSkinnedMeshController = AddComponent<SkinnedMeshController>();
            const auto pathName = ResourceInfo::GetUIPathFileName(tag);
            pUIImage = new UIImage(pathName.first, pathName.second, Vector3::ZERO, this, nullptr);
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

//for animation test 
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

bool Item::HasFinishEvent() const
{
    return pSkinnedMeshController->HasFinishEvent();
}

