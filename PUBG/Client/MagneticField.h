#pragma once
#include "IObject.h"
class MagneticField :
    public IObject
{
private:
    float           m_Radius;
    float           m_ReduceSpeed;
    float           m_DamageMagnitute;

    LPD3DXMESH      m_MagneticField;

    std::chrono::system_clock::time_point m_start;
    float m_coolDown;
    const float m_coolTime;
    
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
};

