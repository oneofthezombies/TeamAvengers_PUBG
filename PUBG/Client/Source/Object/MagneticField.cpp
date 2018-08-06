#include "stdafx.h"
#include "MagneticField.h"
#include "ComponentTransform.h"
#include "Character.h"
#include "UIText.h"
#include "UIImage.h"
#include "ScenePlay.h"

MagneticField::MagneticField()
    : IObject(TAG_OBJECT::MagneticField)
    , m_Position(Vector3::ZERO)

    , m_Radius(30000.0f) // origin
    //, m_Radius(8000.0f) // for recording

    , m_DamageMagnitute(1.0f)
    , m_damageCoolDown(0.0f)
    , m_damageCoolTime(2.0f)
    , m_MagneticField(nullptr)

    , m_coolTime(5.0f * 60.0f) // origin
    //, m_coolTime(80.0f) // for recording

    , m_coolDown(0.0f)
    , m_isMoving(false)
    , m_isPrintOnce(false)
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


    //ó�� random���� position ����
    m_Position = finalDestination[rand() % 5]; // origin
    //m_Position = finalDestination[0]; // for record
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
    //const float dt = 0.0f; // for recording

    ScenePlay* currentScene = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    Character* pPlayer = currentScene->GetPlayer();
    auto& inGameUI = pPlayer->GetInGameUI();
    
    if (m_coolDown <= 0.0f)
    {
        // do
        if (m_Radius > 1000.0f)//�� ���Ϸδ� �� �پ� ��
        {
            m_Radius -= dt * 200.0f;
            m_isMoving = true;
        }
    }
    else
    {
        m_coolDown -= dt; //�ʰ� �پ��� �κ�
    }

    int castToInt = static_cast<int>(m_Radius);

    if (m_isMoving && castToInt ==/*1500 */10000) // origin
    //if (m_isMoving && castToInt == 5000) // for recording
    {
        m_coolDown = m_coolTime;
        m_DamageMagnitute = 2.0f;
        m_isMoving = false;

        inGameUI.pTimeBg->SetIsRender(true);       //���ѽð��� �׸���
        inGameUI.pTimeZeroImg->SetIsRender(false); //�ﰢ���� ������
        m_isPrintOnce = false;
    }
    else if (m_isMoving &&castToInt == /*1200*/5000) // origin
    //else if (m_isMoving && castToInt == 2000) // for recording
    {
        m_coolDown = m_coolTime;
        m_DamageMagnitute = 3.0f;
        m_isMoving = false;

        inGameUI.pTimeBg->SetIsRender(true);       //���ѽð��� �׸���
        inGameUI.pTimeZeroImg->SetIsRender(false); //�ﰢ���� ������
        m_isPrintOnce = false;
    }
 
    //text UI
    const int nCoolDown = static_cast<int>(m_coolDown);
    const int minutes = nCoolDown / 60;
    const int seconds = nCoolDown % 60;
    Debug << "remaining time : " << minutes << " : " << seconds << endl;

    //for InGameUI
    //�̴ϸ� �� ���ѽð� ǥ��
    updateTime(minutes, seconds); // origin
    //updateTime(0, 0); // for recording
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

void MagneticField::updateTime(const int minutes, const int seconds)
{
    ScenePlay* currentScene = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    Character* pPlayer = currentScene->GetPlayer();
    auto& inGameUI = pPlayer->GetInGameUI();

    inGameUI.pMagneticFieldTimeText->SetText(to_string(minutes) + ":" + to_string(seconds));

    if (pPlayer->GetTotalInventory().isOpened == false)
    {
        //�κ��丮�� ���������ʰ�
        // - 0:00�� �ƴϸ� ���� �ð��� �����ش�
        if (minutes != 0 || seconds != 0)
        {
            inGameUI.pTimeBg->SetIsRender(true);
        }
        // - 0:00�̸� !�ﰢ���� �����ش�
        if (minutes == 0 && seconds == 0)
        {
            inGameUI.pTimeZeroImg->SetIsRender(true);
        }
    }

    //-��� ���� ����
    if (seconds == 0)
    {
        if (minutes == 0)
        {
            if (!m_isPrintOnce)
            {
                string str = "��� ������ ���ѵǰ� �ֽ��ϴ�!";
                inGameUI.pMagneticFieldInfoText->SetText(str, inGameUI.pMagneticFieldInfoShadowText);

                //!�ﰢ��
                inGameUI.pTimeBg->SetIsRender(false);     //���ѽð��� ������
                inGameUI.pTimeZeroImg->SetIsRender(true); //�ﰢ���� �׸���
                m_isPrintOnce = true;
            }
        }
        else if (minutes == 4 || minutes == 2 || minutes == 1)
        {
            string str = "��� ������ ���ѵ˴ϴ�. " + to_string(minutes) + "�� ���ҽ��ϴ�!";
            inGameUI.pMagneticFieldInfoText->SetText(str, inGameUI.pMagneticFieldInfoShadowText);
        }
    }
    else if (minutes == 1 && seconds == 30)
    {
        string str = "��� ������ ���ѵ˴ϴ�. " + to_string(minutes) + "�� " + to_string(seconds) + "�� ���ҽ��ϴ�!";
        inGameUI.pMagneticFieldInfoText->SetText(str, inGameUI.pMagneticFieldInfoShadowText);
    }
    else if (minutes == 0)
    {
        if (seconds == 30)
        {
            string str = "��� ������ ���ѵ˴ϴ�. " + to_string(seconds) + "�� ���ҽ��ϴ�!";
            inGameUI.pMagneticFieldInfoText->SetText(str, inGameUI.pMagneticFieldInfoShadowText);
        }
        else if (seconds == 10)
        {
            string str = "��� ������ ���ѵ˴ϴ�. " + to_string(seconds) + "�� ���ҽ��ϴ�!";
            inGameUI.pMagneticFieldInfoText->SetText(str, inGameUI.pMagneticFieldInfoShadowText);
        }
        else if (seconds == 5)
        {
            string str = "��� ������ ���ѵ˴ϴ�. " + to_string(seconds) + "�� ���ҽ��ϴ�!";
            inGameUI.pMagneticFieldInfoText->SetText(str, inGameUI.pMagneticFieldInfoShadowText);
        }
    }
}
