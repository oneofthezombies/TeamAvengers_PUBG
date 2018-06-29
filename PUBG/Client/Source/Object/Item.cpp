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
    , m_numBullet(0)

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

TAG_RES_STATIC Item::GetTagResStatic() const
{
    return m_tagResStatic;
}

void Item::SetDurability(const float durability)
{
    m_durability = durability;
}

void Item::SetNumBullet(const int numBullet)
{
    m_numBullet = numBullet;
}

void Item::setup(const TAG_RES_STATIC tag)
{
    m_durability = ItemInfo::GetDefaultDurability(tag);

}
