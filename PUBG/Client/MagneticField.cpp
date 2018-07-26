#include "stdafx.h"
#include "MagneticField.h"
#include "ComponentTransform.h"



MagneticField::MagneticField()
    :IObject(TAG_OBJECT::Idle)
    , m_Radius(25600.0f)
    , m_ReduceSpeed(1.0f)
    , m_DamageMagnitute(10.0f)
{

}


MagneticField::~MagneticField()
{
}

void MagneticField::setFinalDestination()
{
    std::vector<D3DXVECTOR3> finalDestination;
    finalDestination.resize(5);

    finalDestination[0] = D3DXVECTOR3(0, 0, 0);
    finalDestination[1] = D3DXVECTOR3(0, 0, 0);
    finalDestination[2] = D3DXVECTOR3(0, 0, 0);
    finalDestination[3] = D3DXVECTOR3(0, 0, 0);
    finalDestination[4] = D3DXVECTOR3(0, 0, 0);
    //처음 random으로 position 설정
    GetTransform()->SetPosition(finalDestination[rand() % 5]);


}


void MagneticField::Init()
{
    setFinalDestination();
}

void MagneticField::OnUpdate()
{
}

void MagneticField::OnRender()
{
}
