#include "stdafx.h"
#include "Character.h"
//for UI
#include "UIImage.h"
#include "UIText.h"

Character::InGameUI::InGameUI()
    : m_background(nullptr)
{
}

Character::InGameUI::~InGameUI()
{
}

void Character::InGameUI::Init()
{
    //���� ���
    m_background = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        nullptr
    );
    UI()()->RegisterUIObject(m_background);

    //Compass
    auto compassBg = new UIImage(
        "./Resource/UI/InGame/",
        "compass_bg.png",
        D3DXVECTOR3(420.0f, 28.0f, 0.0f),
        nullptr,
        m_background
    );

    new UIImage(
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
        m_background
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
    new UIImage(
        "./Resource/UI/InGame/",
        "equipment_bag01.png",
        D3DXVECTOR3(502.0f, 647.0f, 0.0f),
        nullptr,
        m_background
    );

    new UIImage(
        "./Resource/UI/InGame/",
        "equipment_helmet01.png",
        D3DXVECTOR3(526.0f, 647.0f, 0.0f),
        nullptr,
        m_background
    );

    new UIImage(
        "./Resource/UI/InGame/",
        "equipment_vest01.png",
        D3DXVECTOR3(550.0f, 647.0f, 0.0f),
        nullptr,
        m_background
    );

    //hp
    auto hpBg = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_BG_v3.png",
        D3DXVECTOR3(502.0f, 684.0f, 0.0f),
        nullptr,
        m_background);

    auto hpRed = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_bar_red.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        hpBg);

    auto hpWhite = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_bar_v2.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        hpRed);

    //�� ��� �ڵ�
    hpRed->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    hpWhite->SetSize(D3DXVECTOR2(250.0f, 17.0f));


    //���� ��, �Ѿ� ����
    auto ammoBg = new UIImage(
        "./Resource/UI/InGame/",
        "ammo_info.png",
        D3DXVECTOR3(576.0f, 647.0f, 0.0f),
        nullptr,
        m_background);

    auto ammoReloadText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoReload),
        D3DXVECTOR2(130.0f, 28.0f),
        string("30"),
        D3DCOLOR_XRGB(255, 255, 255),
        ammoBg);

    ammoReloadText->SetDrawTextFormat(DT_CENTER);
    ammoReloadText->SetPosition(D3DXVECTOR3(1.9f, 0.1f, 0.0f));

    auto ammoTotalText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoTotalNum),
        D3DXVECTOR2(130.0f, 28.0f),
        string("99"),
        D3DCOLOR_XRGB(180, 180, 180),
        ammoBg);

    ammoTotalText->SetDrawTextFormat(DT_LEFT);
    ammoTotalText->SetPosition(D3DXVECTOR3(95.0, 8.0f, 0.0f));


    //����, ���� ��
    auto survivalBg = new UIImage(
        "./Resource/UI/InGame/",
        "survival_text_bg.png",
        D3DXVECTOR3(1216.0f, 20.0f, 0.0f),
        nullptr,
        m_background
    );

    auto survival = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvival),
        D3DXVECTOR2(40.0f, 26.0f),
        string("����"),
        D3DCOLOR_XRGB(180, 180, 180),
        survivalBg
    );
    survival->SetDrawTextFormat(DT_CENTER);
    survival->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

    auto survivalNumBg = new UIImage(
        "./Resource/UI/InGame/",
        "survival_num_bg.png",
        D3DXVECTOR3(1190.0f, 20.0f, 0.0f),
        nullptr,
        m_background
    );

    auto survivalText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(26.0f, 26.0f),
        string("57"),
        D3DCOLOR_XRGB(255, 255, 255),
        survivalNumBg);
    survivalText->SetDrawTextFormat(DT_CENTER);
    survivalText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));

    //���̵�, ���ӹ���
    auto idText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameID),
        D3DXVECTOR2(87.0f, 9.0f),
        string("HelloWoori"),
        D3DCOLOR_XRGB(180, 180, 180),
        m_background
    );
    idText->SetDrawTextFormat(DT_CENTER);
    idText->SetPosition(D3DXVECTOR3(598.0f, 705.0f, 0.0f));

    //ų

    auto killNumShadow = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillNum),
        D3DXVECTOR2(60.0f, 30.0f),
        string("2") + string(" ų"),
        D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f),
        m_background);
    killNumShadow->SetDrawTextFormat(DT_CENTER);
    killNumShadow->SetPosition(D3DXVECTOR3(612.0f + 1.0f, 504.0f + 1.0f, 0.0f));

    auto killNum = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillNum),
        D3DXVECTOR2(60.0f, 30.0f),
        string("2") + string(" ų"),
        D3DCOLOR_XRGB(255, 0, 0),
        m_background);
    killNum->SetDrawTextFormat(DT_CENTER);
    killNum->SetPosition(D3DXVECTOR3(612.0f, 504.0f, 0.0f));

    auto killTextShadow = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillText),
        D3DXVECTOR2(400.0f, 20.0f),
        string("����� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"),
        D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f),
        m_background);
    killTextShadow->SetDrawTextFormat(DT_CENTER);
    killTextShadow->SetPosition(D3DXVECTOR3(440.0f + 1.0f, 480.0f + 1.0f, 0.0f));

    auto killText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillText),
        D3DXVECTOR2(400.0f, 20.0f),
        string("����� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
        m_background);
    killText->SetDrawTextFormat(DT_CENTER);
    killText->SetPosition(D3DXVECTOR3(440.0f, 480.0f, 0.0f));
}

void Character::InGameUI::Destroy()
{
    UI()()->Destroy(m_background);
}

void Character::InGameUI::Update()
{
}

void Character::InGameUI::Render()
{
}