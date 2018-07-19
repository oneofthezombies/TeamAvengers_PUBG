#include "stdafx.h"
#include "DeathDropBox.h"
#include "EffectMeshRenderer.h"
#include "Item.h"

DeathDropBox::DeathDropBox()
    : IObject(TAG_OBJECT::DeathDropBox)
    , pEffectMeshRenderer(nullptr)
{
    const float max = std::numeric_limits<float>::max();
    GetTransform()->SetPosition(Vector3::ONE * max);

    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(TAG_RES_STATIC::DeathDropBox);
    m_boundingSphere = pEffectMeshRenderer->GetBoundingSphere();
}

DeathDropBox::~DeathDropBox()
{
}

void DeathDropBox::OnUpdate()
{
}

void DeathDropBox::OnRender()
{
    pEffectMeshRenderer->Render(
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World, 
            &GetTransform()->GetTransformationMatrix());
    });

    m_boundingSphere.position = GetTransform()->GetPosition();
    m_boundingSphere.Render();
}

void DeathDropBox::SetPosition(const D3DXVECTOR3& position)
{
    GetTransform()->SetPosition(position);
    GetTransform()->Update();
}

void DeathDropBox::SetItems(Character* pCharacter)
{
    Character::TotalInventory& inven = 
        pCharacter->GetTotalInventory();

    auto setItem = [this](Item** ppItem)
    {
        if (ppItem && *ppItem)
        {
            (*ppItem)->SetDeathDropBox(this);
            (*ppItem)->SetState(false);
            m_items.emplace_back(*ppItem);
            *ppItem = nullptr;
        }
    };

    setItem(&inven.m_pHand);
    setItem(&inven.m_pWeaponPrimary);
    setItem(&inven.m_pWeaponSecondary);
    setItem(&inven.m_pEquipHead);
    setItem(&inven.m_pEquipArmor);
    setItem(&inven.m_pEquipBack);

    for (auto& kv : inven.m_mapInventory)
    {
        std::vector<Item*>& items = kv.second;
        for (Item* p : items)
        {
            p->SetDeathDropBox(this);
            p->SetState(false);
            m_items.emplace_back(p);
        }
        items.clear();
    }
}

void DeathDropBox::DeleteThisItem(Item* pItem)
{
    assert(pItem && "DeathDropBox::DeleteThisItem()");

    for (auto it = m_items.begin(); it != m_items.end();)
    {
        if (*it == pItem)
        {
            it = m_items.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
}

const std::vector<Item*>& DeathDropBox::GetItems() const
{
    return m_items;
}
