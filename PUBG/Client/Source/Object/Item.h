#pragma once
#include "IObject.h"

class EffectMeshRenderer;
class SkinnedMeshController;
class UIImage;
struct FramePtr;

class Item : public IObject
{
public:
    static const int DEFAULT_NUM_BULLET  = 30;
    static const int DEFAULT_NUM_BANDAGE = 5;

    static const float DEFAULT_BLENDING_TIME;
    static const float DEFAULT_NEXT_WEIGHT;
    static const float DEFAULT_FINISH_EVENT_AGO_TIME;
    static const float DEFAULT_POSITION;

private:
    const TAG_RES_STATIC m_tagResStatic;
    
    float m_durability; //내구성
    int   m_count;      //아이템의 개수
    
    //for Rifle
    int   m_numBullet;  //장전된 총알의 개수
    bool  m_auto;

    EffectMeshRenderer*    pEffectMeshRenderer;
    bool                   m_isRenderEffectMesh;

    SkinnedMeshController* pSkinnedMeshController;
    bool                   m_isRenderSkinnedMesh;

    //ui
    UIImage* m_pUIImage;
    UIImage* m_pUIImage2;
    UIText * pUIText;
    bool    m_inInventory;
 
    //총알이 나갈 위치
    Frame* pGunBolt;

    FramePtr* m_pFramePtr;
    
private:
    void setup(const TAG_RES_STATIC tag);
    void setGlobalVariable(LPD3DXEFFECT pEffect);
    void addAnimationBackupFrameForEquip();
    void setFramePtr();

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

    void SetIsRenderUIImage(const bool isRenderUIImage);
    bool IsRenderUIImage() const;

    void SetPosition(const D3DXVECTOR3& position);
    void SetUIPosition(const D3DXVECTOR2& position);

    //ui text
    UIText* GetUIText();
    UIImage* GetUIImage();
    UIImage* GetUIImage2();
    void SetState(bool state);
    bool GetState();

    //for Rifle
    void SetNumBullet(const int numBullet);
    int GetNumBullet() const;

    Frame* GetGunBolt() const;
    void ChangeAuto();
    bool GetAuto();

    void UpdateAnimation();
    void UpdateModel();

    void UpdateBone(Item* pHand, const float headRot, const float waistRot);

    SkinnedMesh* GetSkinnedMesh() const;

    //for 아이템 자체 애니메이션
    void Set(
        const TAG_ANIM_WEAPON tag,
        const bool  isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f,
        const float position = 0.0f);

    void Set(
        const TAG_ANIM_WEAPON tag,
        const bool  isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void Set(
        const TAG_ANIM_WEAPON tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void OnKar98kReload(const int numReload);
    void OnKar98kReloadEnd();

    bool HasFinishEvent() const;

    //for 장비 애니메이션
    void Set(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f,
        const float position = 0.0f);

    void Set(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void Set(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void SetTrackPosition(const std::size_t index, const double position);
    void SetSubTrackPosition(const std::size_t index, const double position);
};
