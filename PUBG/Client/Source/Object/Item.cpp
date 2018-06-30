#include "stdafx.h"
#include "Item.h"
#include "ItemInfo.h"
#include "EffectMeshRenderer.h"
#include "SkinnedMeshController.h"
#include "DirectionalLight.h"

Item::Item(
    const TAG_RES_STATIC tag, 
    const D3DXVECTOR3& position, 
    const D3DXVECTOR3& rotation, 
    const D3DXVECTOR3& scale)
    : IObject()
    , m_tagResStatic(tag)
    , m_isRenderEffectMesh(true)
    , m_durability(0.0f)
    , m_count(0)

    , pEffectMeshRenderer(nullptr)
{
    Transform* pTr = GetTransform();
    pTr->SetPosition(position);
    pTr->SetRotation(rotation);
    pTr->SetScale(scale);
    pTr->Update();

    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(tag);

    setup(tag);
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
        pEffectMeshRenderer->Render
        (
            [this](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });
}

void Item::setup(const TAG_RES_STATIC tag)
{
    //내구성 셋팅
    m_durability = ItemInfo::GetDefaultDurability(tag);

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


////////////////////////////////Weapon
Weapon::Weapon(
    const TAG_RES_STATIC tag,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const D3DXVECTOR3& scale)
    : Item(tag, position, rotation, scale)
    , m_isRenderSkinnedMesh(false)
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
}

Weapon::~Weapon()
{

}

void Weapon::OnRender()
{
    if (m_isRenderEffectMesh && !m_isRenderSkinnedMesh)
        pEffectMeshRenderer->Render(
            [this](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });
    
    if (!m_isRenderEffectMesh && m_isRenderSkinnedMesh)
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
}

void Weapon::SetIsRenderSkinnedMesh(const bool isRenderSkinnedMesh)
{
    m_isRenderSkinnedMesh = isRenderSkinnedMesh;
}

bool Weapon::IsRenderSkinnedMesh() const
{
    return m_isRenderSkinnedMesh;
}

