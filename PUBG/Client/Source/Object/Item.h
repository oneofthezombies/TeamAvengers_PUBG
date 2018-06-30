#pragma once
#include "IObject.h"

class EffectMeshRenderer;
class SkinnedMeshController;

class Item : public IObject
{
public:
    static const int DEFAULT_NUM_BULLET  = 30;
    static const int DEFAULT_NUM_BANDAGE = 5;

protected:
    const TAG_RES_STATIC m_tagResStatic;
    
    float m_durability; //내구성
    int   m_count;      //아이템의 개수

    EffectMeshRenderer* pEffectMeshRenderer;
    bool  m_isRenderEffectMesh;

protected:
    void setup(const TAG_RES_STATIC tag);

public:
             Item(
                 const TAG_RES_STATIC tag, 
                 const D3DXVECTOR3& position, 
                 const D3DXVECTOR3& rotation, 
                 const D3DXVECTOR3& scale);
    virtual ~Item();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    TAG_RES_STATIC GetTagResStatic() const;

    void SetDurability(const float durability);
    float GetDurability() const;

    void SetCount(const int count);
    int GetCount() const;

    void SetIsRenderEffectMesh(const bool isRenderEffectMesh);
    bool IsRenderEffectMesh() const;
};

class Weapon : public Item
{
private:
    SkinnedMeshController* pSkinnedMeshController;
    bool m_isRenderSkinnedMesh;

public:
             Weapon(
                 const TAG_RES_STATIC tag,
                 const D3DXVECTOR3& position,
                 const D3DXVECTOR3& rotation,
                 const D3DXVECTOR3& scale);
    virtual ~Weapon();

    virtual void OnRender() override;

    void SetIsRenderSkinnedMesh(const bool isRenderSkinnedMesh);
    bool IsRenderSkinnedMesh() const;
};