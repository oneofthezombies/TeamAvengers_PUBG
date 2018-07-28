#pragma once
#include "IObject.h"
class MagneticField :
    public IObject
{
private:
    float                                   m_Radius;
    float                                   m_DamageMagnitute;

    LPD3DXMESH                              m_MagneticField;
            
    float                                   m_coolDown;
    const float                             m_coolTime;
    
    bool                                    m_isMoving;
    
    
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

    bool IsInside(const D3DXVECTOR3 pos);
};
