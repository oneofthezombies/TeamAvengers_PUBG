#pragma once
#include "IObject.h"

class EffectMeshRenderer;
class SkinnedMeshController;
class UIImage;

class Item : public IObject
{
public:
    static const int DEFAULT_NUM_BULLET  = 30;
    static const int DEFAULT_NUM_BANDAGE = 5;

private:
    const TAG_RES_STATIC m_tagResStatic;
    
    float m_durability; //내구성
    int   m_count;      //아이템의 개수
    //for Rifle
    int   m_numBullet;  //장전된 총알의 개수

    EffectMeshRenderer* pEffectMeshRenderer;
    bool                m_isRenderEffectMesh;

    SkinnedMeshController* pSkinnedMeshController;
    bool                   m_isRenderSkinnedMesh;

    UIImage* pUIImage;

    //총알이 나갈 위치 테스트
    Frame* pGunBolt;
    

private:
    void setup(const TAG_RES_STATIC tag);
    void setGlobalVariable(LPD3DXEFFECT pEffect);

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

    void SetIsRenderSkinnedMesh(const bool isRenderSkinnedMesh);
    bool IsRenderSkinnedMesh() const;

    void SetInRenderUIImage(const bool isRenderUIImage);
    bool IsRenderUIImage() const;

    void SetPosition(const D3DXVECTOR3& position);
    void SetUIPosition(const D3DXVECTOR2& position);

    //for Rifle
    void SetNumBullet(const int numBullet);
    int GetNumBullet() const;

    Frame* GetGunBolt() const
    {
        return pGunBolt;
    }

    void UpdateModel();
};
