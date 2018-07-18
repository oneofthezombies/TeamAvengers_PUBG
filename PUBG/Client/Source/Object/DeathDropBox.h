#pragma once

class EffectMeshRenderer;
class Item;

class DeathDropBox : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;
    std::vector<Item*> m_items;

public:
             DeathDropBox();
    virtual ~DeathDropBox();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void SetPosition(const D3DXVECTOR3& position);
    void SetItems(Character* pCharacter);
    void DeleteThisItem(Item* pItem);
    const std::vector<Item*>& GetItems() const;
};