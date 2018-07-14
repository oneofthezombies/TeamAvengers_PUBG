#include "stdafx.h"
#include "Character.h"
//for UI
#include "UIImage.h"
#include "UIText.h"

Character::InGameUI::InGameUI()
    : m_pBackground(nullptr)

    //Image ===================
    , pCompass(nullptr)

    , pBagImg(nullptr)
    , pHelmetImg(nullptr)
    , pVestImg(nullptr)

    , pHpRedImg(nullptr)
    , pHpWhiteImg(nullptr)

    , pPrimaryWeaponImg(nullptr)
    , pSecondaryWeaponImg(nullptr)

    //Text ====================
    , pAmmoReloadText(nullptr)
    , pAmmoNumText(nullptr)
    , pFireModeText(nullptr)

    , pSurvivalNumText(nullptr)
    , pKillNumUpText(nullptr)

    , pIdText(nullptr)

    , pKillNumText(nullptr)
    , pKillText(nullptr)
    , pKillNumTextShadow(nullptr)
    , pKillTextShadow(nullptr)

    , pInfoText(nullptr)
    , pInfoTextShadow(nullptr)

    , pKillLog1(nullptr)
    , pKillLog2(nullptr)

    //=========================
    , COOL_TIME(4.0)
{
}

Character::InGameUI::~InGameUI()
{
}

void Character::InGameUI::Destroy()
{
    UI()()->Destroy(m_pBackground);
}

void Character::InGameUI::Init()
{
    m_coolDown = COOL_TIME;
    //���� ���
    m_pBackground = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        nullptr
    );
    UI()()->RegisterUIObject(m_pBackground);

    //Compass
    auto compassBg = new UIImage(
        "./Resource/UI/InGame/",
        "compass_bg.png",
        D3DXVECTOR3(420.0f, 28.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    pCompass = new UIImage(
        "./Resource/UI/InGame/",
        "compass.png",
        D3DXVECTOR3(-178.0f, 0.0f, 0.0f),
        nullptr,
        compassBg
    );

    auto compassArrowBg = new UIImage(
        "./Resource/UI/InGame/",
        "compass_arraw_bg.png",
        D3DXVECTOR3(420.0f, 8.75f, 0.0f),
        nullptr,
        m_pBackground
    );

    new UIImage(
        "./Resource/UI/InGame/",
        "compass_arrow.png",
        D3DXVECTOR3(-178.0f, 0.0f, 0.0f),
        nullptr,
        compassArrowBg
    );

    //for test
    //compassBg->SetIsRender(false);
    //�θ� ������ �ڽĵ� �Բ� �Ⱥ��δ�

    //�����
    pBagImg = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_bag01.png",
        D3DXVECTOR3(502.0f, 647.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    pHelmetImg = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_helmet01.png",
        D3DXVECTOR3(526.0f, 647.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    pVestImg = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_vest01.png",
        D3DXVECTOR3(550.0f, 647.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    //hp
    auto hpBg = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_BG_v3.png",
        D3DXVECTOR3(502.0f, 684.0f, 0.0f),
        nullptr,
        m_pBackground);

    pHpRedImg = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_bar_red.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        hpBg);

    pHpWhiteImg = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_bar_v2.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        pHpRedImg);

    //�� ��� �ڵ�
    pHpRedImg->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    pHpWhiteImg->SetSize(D3DXVECTOR2(250.0f, 17.0f));


    //���� ��, �Ѿ� ����
    auto ammoBg = new UIImage(
        "./Resource/UI/InGame/",
        "ammo_info.png",
        D3DXVECTOR3(576.0f, 647.0f, 0.0f),
        nullptr,
        m_pBackground);

    pAmmoReloadText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoReload),
        D3DXVECTOR2(130.0f, 28.0f),
        string("30"),
        D3DCOLOR_XRGB(255, 255, 255),
        ammoBg);
    pAmmoReloadText->SetDrawTextFormat(DT_CENTER);
    pAmmoReloadText->SetPosition(D3DXVECTOR3(1.9f, 0.1f, 0.0f));

    pAmmoNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoTotalNum),
        D3DXVECTOR2(130.0f, 28.0f),
        string("99"),
        D3DCOLOR_XRGB(180, 180, 180),
        ammoBg);
    pAmmoNumText->SetDrawTextFormat(DT_LEFT);
    pAmmoNumText->SetPosition(D3DXVECTOR3(95.0, 5.0f, 0.0f));

    //�߻���
    pFireModeText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameFireMode),
        D3DXVECTOR2(130.0f, 28.0f),
        string("����"),
        D3DCOLOR_XRGB(180, 180, 180),
        ammoBg);
    pFireModeText->SetDrawTextFormat(DT_LEFT);
    pFireModeText->SetPosition(D3DXVECTOR3(10.0, 8.0f, 0.0f));

    //����, ���� ��
    auto survivalBg = new UIImage(
        "./Resource/UI/InGame/",
        "survival_text_bg.png",
        D3DXVECTOR3(1216.0f, 20.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    auto survivalText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvival),
        D3DXVECTOR2(40.0f, 26.0f),
        string("����"),
        D3DCOLOR_XRGB(180, 180, 180),
        survivalBg
    );
    survivalText->SetDrawTextFormat(DT_CENTER);
    survivalText->SetPosition(D3DXVECTOR3(0.0f, 4.0f, 0.0f));

    auto survivalNumBg = new UIImage(
        "./Resource/UI/InGame/",
        "survival_num_bg.png",
        D3DXVECTOR3(1190.0f, 20.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    pSurvivalNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(26.0f, 26.0f),
        string("57"),
        D3DCOLOR_XRGB(255, 255, 255),
        survivalNumBg);
    pSurvivalNumText->SetDrawTextFormat(DT_CENTER);
    pSurvivalNumText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    
    //ų (ȭ�� ������ ���)
    auto killTextUpBg = new UIImage(
        "./Resource/UI/InGame/",
        "kill_text_up_bg.png",
        D3DXVECTOR3(1145.0f, 20.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    auto killUpText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvival),
        D3DXVECTOR2(24.0f, 26.0f),
        string("ų"),
        D3DCOLOR_XRGB(180, 180, 180),
        killTextUpBg
    );
    killUpText->SetDrawTextFormat(DT_CENTER);
    killUpText->SetPosition(D3DXVECTOR3(0.0f, 4.0f, 0.0f));

    auto killNumUpBg = new UIImage(
        "./Resource/UI/InGame/",
        "kill_num_up_bg.png",
        D3DXVECTOR3(1128.0f, 20.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    pKillNumUpText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(17.0f, 26.0f),
        string("0"),
        D3DCOLOR_XRGB(255, 255, 255),
        killNumUpBg);
    pKillNumUpText->SetDrawTextFormat(DT_CENTER);
    pKillNumUpText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));


    //���̵�, ���ӹ���
    pIdText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameID),
        D3DXVECTOR2(87.0f, 9.0f),
        string("HelloWoori"),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
        m_pBackground
    );
    pIdText->SetDrawTextFormat(DT_CENTER);
    pIdText->SetPosition(D3DXVECTOR3(598.0f, 705.0f, 0.0f));

    //ų (���� ȭ�� �߾�)
    setTextWithShadow(
        pKillNumText,
        pKillNumTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameKillNum),
        D3DXVECTOR2(60.0f, 30.0f),
        string("2 ų"),
        D3DCOLOR_XRGB(216, 0, 0),
        m_pBackground,
        D3DXVECTOR3(612.0f, 504.0f, 0.0f));

    setTextWithShadow(
        pKillText,
        pKillTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(400.0f, 20.0f),
        string("����� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
        m_pBackground,
        D3DXVECTOR3(440.0f, 480.0f, 0.0f));

    //������ ��� �� �ȳ�����
    setTextWithShadow(
        pInfoText,
        pInfoTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(252.0f, 20.0f),
        string(""),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
        m_pBackground,
        D3DXVECTOR3(510.0f, 579.0f, 0.0f));

    //ų�α�
    pKillLog1 = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string("HelloWoori�� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
        m_pBackground);
    pKillLog1->SetDrawTextFormat(DT_RIGHT);
    pKillLog1->SetPosition(D3DXVECTOR3(856.0f, 52.0f, 0.0f));

    pKillLog2  = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string("John�� QBZ(��)�� ���� ootz��(��) ����߽��ϴ�"),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
        m_pBackground);
    pKillLog2->SetDrawTextFormat(DT_RIGHT);
    pKillLog2->SetPosition(D3DXVECTOR3(856.0f, 52.0f + 20.0f, 0.0f));

    //��
    auto primaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        D3DXVECTOR3(970.0f, 670.0f, 0.0f),
        nullptr,
        m_pBackground);

    auto secondaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        D3DXVECTOR3(970.0f, 632.0f, 0.0f),
        nullptr,
        m_pBackground);

    pPrimaryWeaponImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_QBZ95.png",
        D3DXVECTOR3(28.0f, 0.0f, 0.0f),
        nullptr,
        primaryWeaponBg);

    pSecondaryWeaponImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_kar98k.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        secondaryWeaponBg);


    //��
    new UIImage(
        "./Resource/UI/InGame/",
        "map_bg.png",
        D3DXVECTOR3(1085.0f, 530.0f, 0.0f),
        nullptr,
        m_pBackground
    );
}

void Character::InGameUI::Update()
{
    //���� ������ �޾Ƽ� string�� �־�����
    //for test
    pKillNumUpText->SetText(string("9"));

    //�����ð��� ������ �ش� ���� ����
    if (pInfoText->GetText() != "")
    {
        float t = m_coolDown -= Time()()->GetDeltaTime();
        cout << t << endl;
        if (m_coolDown <= 0)
        {
            pInfoText->SetText("");
            pInfoTextShadow->SetText("");
            m_coolDown = COOL_TIME;
        }
    }
}

void Character::InGameUI::Render()
{
}

void Character::InGameUI::setTextWithShadow(
    UIText*& pText,
    UIText*& pTextShadow,
    const LPD3DXFONT font,
    const D3DXVECTOR2& size,
    const string& str,
    const D3DCOLOR color,
    UIObject* pParent,
    const D3DXVECTOR3& position)
{
    pTextShadow = new UIText(
        font,
        size,
        str,
        D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f),
        pParent);
    pTextShadow->SetDrawTextFormat(DT_CENTER);
    pTextShadow->SetPosition(D3DXVECTOR3(position.x + 1.0f, position.y + 1.0f, position.z));

    pText = new UIText(
        font,
        size,
        str,
        color,
        pParent);
    pText->SetDrawTextFormat(DT_CENTER);
    pText->SetPosition(position);
}