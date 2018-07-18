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

    if (inven.m_pHand)
    {
        inven.m_pHand->SetDeathDropBox(this);
        m_items.emplace_back(inven.m_pHand);
        inven.m_pHand = nullptr;
    }

    if (inven.m_pWeaponPrimary)
    {
        inven.m_pWeaponPrimary->SetDeathDropBox(this);
        m_items.emplace_back(inven.m_pWeaponPrimary);
        inven.m_pWeaponPrimary = nullptr;
    }

    if (inven.m_pWeaponSecondary)
    {
        inven.m_pWeaponSecondary->SetDeathDropBox(this);
        m_items.emplace_back(inven.m_pWeaponSecondary);
        inven.m_pWeaponSecondary = nullptr;
    }

    if (inven.m_pEquipHead)
    {
        inven.m_pEquipHead->SetDeathDropBox(this);
        m_items.emplace_back(inven.m_pEquipHead);
        inven.m_pEquipHead = nullptr;
    }

    if (inven.m_pEquipArmor)
    {
        inven.m_pEquipArmor->SetDeathDropBox(this);
        m_items.emplace_back(inven.m_pEquipArmor);
        inven.m_pEquipArmor = nullptr;
    }

    if (inven.m_pEquipBack)
    {
        inven.m_pEquipBack->SetDeathDropBox(this);
        m_items.emplace_back(inven.m_pEquipBack);
        inven.m_pEquipBack = nullptr;
    }

    for (auto& kv : inven.m_mapInventory)
    {
        std::vector<Item*>& items = kv.second;
        for (Item* p : items)
        {
            p->SetDeathDropBox(this);
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
