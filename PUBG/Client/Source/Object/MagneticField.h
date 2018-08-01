#pragma once
#include "IObject.h"
class MagneticField :
    public IObject
{
private:
    D3DXVECTOR3                             m_Position;
    float                                   m_Radius;
    float                                   m_DamageMagnitute;
    float                                   m_damageCoolDown;
    const float                             m_damageCoolTime;

    LPD3DXMESH                              m_MagneticField;
            
    float                                   m_coolDown;
    const float                             m_coolTime;
    
    bool                                    m_isMoving;
    bool                                    m_isPrintOnce;
    
private:
    void setFinalDestination();

public:
    MagneticField();
    ~MagneticField();

    void Init();
    // Inherited via IObject
    virtual void OnUpdate() override;
    virtual void OnRender() override;

    //처음 random 으로 5곳 중 한곳을 설정
    //5분 후 부터 줄어들기 
    //총 5번 줄어들기 
    //Damage를 받는 정도를 점점 크게 한다
    //reducing speed

    float GetRadius() const;
    float GetDamage() const;

    bool IsDamageTime(const float dt);
    bool IsInside(const D3DXVECTOR3 pos);

    void updateTime(const int minutes, const int seconds);
};
