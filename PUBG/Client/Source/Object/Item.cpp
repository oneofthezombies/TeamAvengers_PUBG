#include "stdafx.h"
#include "Item.h"
#include "ItemInfo.h"
#include "EffectMeshRenderer.h"

Item::Item(
    const TAG_RES_STATIC tag, 
    const D3DXVECTOR3& position, 
    const D3DXVECTOR3& rotation, 
    const D3DXVECTOR3& scale)
    : IObject()
    , m_tagResStatic(tag)
    , m_isInField(true)
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
    pEffectMeshRenderer->Render();
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

void Item::SetIsInField(const bool isInField)
{
    m_isInField = isInField;
}

bool Item::IsInField() const
{
    return m_isInField;
}
