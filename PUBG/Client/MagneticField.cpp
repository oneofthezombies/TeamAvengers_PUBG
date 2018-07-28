#include "stdafx.h"
#include "MagneticField.h"
#include "ComponentTransform.h"


MagneticField::MagneticField()
    :IObject(TAG_OBJECT::MagneticField)
    , m_Position(Vector3::ZERO)
    , m_Radius(25600.0f)/*82712.3*/
    , m_DamageMagnitute(1.0f)
    , m_damageCoolDown(0.0f)
    , m_damageCoolTime(2.0f)
    , m_MagneticField(nullptr)
    , m_coolTime(5.0f * 60.0f)
    /*, m_coolTime(10.0f)*/
    , m_coolDown(0.0f)
    , m_isMoving(false)
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


    //처음 random으로 position 설정
    m_Position = finalDestination[rand() % 5];
    
}


void MagneticField::Init()
{
    D3DXCreateSphere(Device()(), 1.0f, 20, 20, &m_MagneticField, nullptr);
    
    setFinalDestination();

    m_coolDown = m_coolTime;
    m_isMoving = false;
}

void MagneticField::OnUpdate()
{
    Debug << "final Destination : " << m_Position << endl;
    Debug << "Radius : " << m_Radius << endl;
    
    const float dt = Time()()->GetDeltaTime();
    
    if (m_coolDown <= 0.0f)
    {
        // do
        if (m_Radius > 1000.0f)//이 이하로는 안 줄어 듬
        {
            m_Radius -= dt * 100.0f;
            m_isMoving = true;
        }
    }
    else
    {
        m_coolDown -= dt; //초가 줄어드는 부분
    }

    int castToInt = static_cast<int>(m_Radius);

    if (m_isMoving && castToInt ==/*1500 */10000)
    {
        m_coolDown = m_coolTime;
        m_DamageMagnitute = 2.0f;
        m_isMoving = false;
    }
    else if (m_isMoving &&castToInt == /*1200*/5000)
    {
        m_coolDown = m_coolTime;
        m_DamageMagnitute = 3.0f;
        m_isMoving = false;
    }
 
    

    //text UI
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
        &m_Position);

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        m_MagneticField,
        0,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR skyBlue(0.4784f, 0.5686f, 0.9921f, 0.3f);
        pEffect->SetValue("Color", &skyBlue, sizeof skyBlue);
    });
}

float MagneticField::GetRadius() const
{
    return m_Radius;
}

float MagneticField::GetDamage() const
{
    return m_DamageMagnitute;
}

bool MagneticField::IsDamageTime(const float dt)
{
    m_damageCoolDown -= dt;
    if (m_damageCoolDown <= 0.0f)
    {
        m_damageCoolDown = m_damageCoolTime;
        return true;
    }
    return false;
}

bool MagneticField::IsInside(const D3DXVECTOR3 pos)
{
    
    D3DXVECTOR3 length = m_Position - pos;
    float test = D3DXVec3Length(&length);
    return test < m_Radius;
}
