#include "stdafx.h"
#include "MagneticField.h"
#include "ComponentTransform.h"


MagneticField::MagneticField()
    :IObject(TAG_OBJECT::MagneticField)
    , m_Radius(100.0f)
    , m_ReduceSpeed(100.0f)
    , m_DamageMagnitute(10.0f)
    , m_MagneticField(nullptr)
    , m_coolTime(5.0f * 60.0f)
    , m_coolDown(0.0f)
{
}


MagneticField::~MagneticField()
{
    SAFE_RELEASE(m_MagneticField);
}

void MagneticField::setFinalDestination()
{
    std::vector<D3DXVECTOR3> finalDestination;
    finalDestination.resize(5);

    finalDestination[0] = D3DXVECTOR3(12800.0f, 5750.0f, 12800.0f);
    finalDestination[1] = D3DXVECTOR3(7300.0f, 4736.155f, 17250.0f);
    finalDestination[2] = D3DXVECTOR3(8000.0f, 4501.0f, 4500.0f);
    finalDestination[3] = D3DXVECTOR3(20000.0f, 4802.0f, 6500.0f);
    finalDestination[4] = D3DXVECTOR3(17700.0f, 4600.0f, 17500.0f);
    
    //finalDestination[0] = D3DXVECTOR3(12800.0f, 5750.0f, 12800.0f);
    //finalDestination[1] = D3DXVECTOR3(12800.0f, 5750.0f, 12800.0f);
    //finalDestination[2] = D3DXVECTOR3(12800.0f, 5750.0f, 12800.0f);
    //finalDestination[3] = D3DXVECTOR3(12800.0f, 5750.0f, 12800.0f);
    //finalDestination[4] = D3DXVECTOR3(12800.0f, 5750.0f, 12800.0f);


    //처음 random으로 position 설정
    GetTransform()->SetPosition(finalDestination[rand() % 5]);
    
}


void MagneticField::Init()
{
    D3DXCreateSphere(Device()(), m_Radius,20,20,&m_MagneticField, nullptr);
    
    setFinalDestination();
    m_start = std::chrono::system_clock::now();

    m_coolDown = m_coolTime;

}

void MagneticField::OnUpdate()
{
    std::chrono::duration<float> deltaTime = std::chrono::system_clock::now() - m_start;

    //auto current = std::chrono::system_clock::now();
    //auto deltaTime = current - m_start;
    //std::chrono::minutes m(deltaTime.count());
    //std::chrono::seconds s(deltaTime.count());


    std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(deltaTime);
    std::chrono::minutes min = std::chrono::duration_cast<std::chrono::minutes>(deltaTime);
    Debug << "sec : " << sec.count() << endl;
    Debug << "min : " << min.count() << endl;


    m_Radius -=/* m_ReduceSpeed **/ Time()()->GetDeltaTime();
    Debug << "final Destination : " << GetTransform()->GetPosition() << endl;
    Debug << "Radius : " << m_Radius << endl;

    const float dt = Time()()->GetDeltaTime();
    m_coolDown -= dt;
    if (m_coolDown <= 0.0f)
    {
        // do

        m_coolDown = m_coolTime;
    }
    
    const int nCoolDown = static_cast<int>(m_coolDown);
    const int minutes = nCoolDown / 60;
    const int seconds = nCoolDown % 60;
    Debug << "remaining time : " << minutes << " : " << seconds << endl;
}

void MagneticField::OnRender()
{
    D3DXMATRIX m;
    D3DXMatrixTransformation(
        &m,
        nullptr,
        nullptr,
        &D3DXVECTOR3(m_Radius, m_Radius, m_Radius),
        nullptr, nullptr,
        &GetTransform()->GetPosition());

    
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        //Resource()()->GetBoundingSphereMesh(),
        m_MagneticField,
        0,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR blue(0.0f, 0.0f, 1.0f, 0.3f);
        pEffect->SetValue("Color", &blue, sizeof blue);
    });
    
}
