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

    //처음 random 으로 5곳 중 한곳을 설정
    //5분 후 부터 줄어들기 
    //총 5번 줄어들기 
    //Damage를 받는 정도를 점점 크게 한다
    //reducing speed
};

