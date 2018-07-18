#include "stdafx.h"
#include "DeathDropBox.h"
#include "EffectMeshRenderer.h"

DeathDropBox::DeathDropBox()
    : IObject(TAG_OBJECT::DeathDropBox)
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(TAG_RES_STATIC::DeathDropBox);

    const float max = std::numeric_limits<float>::max();
    GetTransform()->SetPosition(Vector3::ONE * max);
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
        m_items.emplace_back(inven.m_pHand);
        inven.m_pHand = nullptr;
    }

    if (inven.m_pWeaponPrimary)
    {
        m_items.emplace_back(inven.m_pWeaponPrimary);
        inven.m_pWeaponPrimary = nullptr;
    }

    if (inven.m_pWeaponSecondary)
    {
        m_items.emplace_back(inven.m_pWeaponSecondary);
        inven.m_pWeaponSecondary = nullptr;
    }

    if (inven.m_pEquipHead)
    {
        m_items.emplace_back(inven.m_pEquipHead);
        inven.m_pEquipHead = nullptr;
    }

    if (inven.m_pEquipArmor)
    {
        m_items.emplace_back(inven.m_pEquipArmor);
        inven.m_pEquipArmor = nullptr;
    }

    if (inven.m_pEquipBack)
    {
        m_items.emplace_back(inven.m_pEquipBack);
        inven.m_pEquipBack = nullptr;
    }

    for (auto& kv : inven.m_mapInventory)
    {
        std::vector<Item*>& items = kv.second;
        m_items.insert(m_items.end(), items.begin(), items.end());

        items.clear();
    }
}
