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

    //ó�� random ���� 5�� �� �Ѱ��� ����
    //5�� �� ���� �پ��� 
    //�� 5�� �پ��� 
    //Damage�� �޴� ������ ���� ũ�� �Ѵ�
    //reducing speed

    float GetRadius() const;
    float GetDamage() const;

    bool IsDamageTime(const float dt);
    bool IsInside(const D3DXVECTOR3 pos);

    void updateTime(const int minutes, const int seconds);
};
