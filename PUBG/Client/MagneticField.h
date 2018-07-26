#pragma once
#include "IObject.h"
class MagneticField :
    public IObject
{
private:
    float m_Radius;
    float m_ReduceSpeed;
    float m_DamageMagnitute;

private:
    MagneticField();
    ~MagneticField();

    void setFinalDestination();
public:

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

