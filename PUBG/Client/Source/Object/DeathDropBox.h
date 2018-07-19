#pragma once

class EffectMeshRenderer;
class Item;

class DeathDropBox : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;
    std::vector<Item*> m_items;
    int                m_index;

private:
    void setPosition(const D3DXVECTOR3& position);
    void setItems(Character* pCharacter);

public:
             DeathDropBox();
    virtual ~DeathDropBox();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void Set(const D3DXVECTOR3& position, Character* pCharacter);

    void  DeleteThisItem(Item* pItem);
    Item* FindItem(const std::string& itemName);
    const std::vector<Item*>& GetItems() const;
    int GetIndex() const;
};