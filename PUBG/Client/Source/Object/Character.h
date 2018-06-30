#pragma once
#include "IObject.h"
#include "TagClientOnly.h"
#include "TagAnimation.h"

class SkinnedMeshController;
class CharacterPart;

class Character : public IObject
{
public:
    struct WaistRotation
    {
        const float LIMIT_OF_ANGLE;
        const float QUANTITY_FACTOR;
        float       m_angle;

        WaistRotation(const float limit, const float factor);
    };

    struct RootTransform
    {
        const float MOVE_SPEED;

        RootTransform(const float moveSpeed);
    };

    struct Info
    {
        Transform*   pTransform;
        D3DXVECTOR3* pRotationForCamera;
        Frame*       pFPP;
        Frame*       pTPP;

        Info();
    };

    struct IsPressing
    {
        bool _LAlt    ;
        bool _LCtrl   ;
        bool _LShift  ;
        bool _W       ;
        bool _S       ;
        bool _A       ;
        bool _D       ;
        bool _Z       ;
        bool _X       ;
        bool _C       ;
        bool _Space   ;

        IsPressing();
        bool operator==(const IsPressing& other) const
        {

            if (_LAlt   != other._LAlt    ) return false;
            if (_LCtrl  != other._LCtrl   ) return false;
            if (_LShift != other._LShift  ) return false;
            if (_W      != other._W       ) return false;
            if (_S      != other._S       ) return false;
            if (_A      != other._A       ) return false;
            if (_D      != other._D       ) return false;
            if (_Z      != other._Z       ) return false;
            if (_X      != other._X       ) return false;
            if (_C      != other._C       ) return false;
            if (_Space  != other._Space   ) return false;
            return true;
        }
        bool operator!=(const IsPressing& other) const
        {
            return !(*this == other);
        }        
    };

private:
    struct FramePtr
    {
        Frame* pWaist;
        Frame* pRoot;
        Frame* pHandGun;
        Frame* pTPP;
        Frame* pFPP;

        FramePtr();
    };

public:
    static const int            NUM_PLAYER = 4;
    static const D3DXQUATERNION OFFSET_ROTATION;

private:
    int                m_index;
    TAG_ANIM_CHARACTER m_animState;

    RootTransform m_rootTransform;
    WaistRotation m_waistRotation;
    FramePtr      m_framePtr;

    // for root position
    LPD3DXMESH m_pSphereMesh;

    SkinnedMeshController* pSkinnedMeshController;
    CharacterPart*         m_pRootCharacterPart;

    D3DXMATRIX m_prevRootModel;
    D3DXVECTOR3 m_rotationForCamera;

    Info m_info;
    IsPressing m_savedInput;
    IsPressing m_currentInput;

private:
    void setFramePtr();
    void subscribeCollisionEvent();

    IsPressing HandleInput(IsPressing& m_isPressing);
    void CameraCharacterRotation(OUT D3DXQUATERNION* rOut);
    void AnimationControl(OUT D3DXVECTOR3* pOut, OUT TAG_ANIM_CHARACTER* tagOut);

    void updateMine();
    void updateOther();
    void updateBone();
    void updateDependency();
    void communicate();

    void rotateWaist(const float quantity);

    bool isMine() const;

    void setAnimation(
        const TAG_ANIM_CHARACTER tag, const bool isBlend, 
        const float currentWeight = 1.0f, const float nextWeight = 0.0f,
        const float blendTime = 0.3f);

    void setInfo();

    D3DXVECTOR3 getRight();
    D3DXVECTOR3 getUp();
    D3DXVECTOR3 getForward();

public:
             Character(const int index);
    virtual ~Character();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void OnCollisionEnter(Collider* pPerpetrator, Collider* pVictim);
    void OnCollisionStay (Collider* pPerpetrator, Collider* pVictim);
    void OnCollisionExit (Collider* pPerpetrator, Collider* pVictim);

    int GetIndex() const;
    TAG_COLLISION GetTagCollisionBody(const int index);
    TAG_COLLISION GetTagCollisionDamage(const int index);
};
