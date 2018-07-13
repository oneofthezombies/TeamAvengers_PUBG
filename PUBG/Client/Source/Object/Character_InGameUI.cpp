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
    //투명 배경
    m_background = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        nullptr
    );
    UI()()->RegisterUIObject(m_background);

    //Compass - 사이즈 조정 필요
    //auto n = new UIImage(
    //    "./Resource/UI/InGame/",
    //    "compass.png",
    //    D3DXVECTOR3(40.0f, 22.0f, 0.0f),
    //    nullptr,
    //    m_background
    //);
    //n->SetIsRender(false);

    //부모를 가리면 자식도 함께 안보인다

    //장비템
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

    //피 닳기 코드
    hpRed->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    hpWhite->SetSize(D3DXVECTOR2(250.0f, 17.0f));


    //장전 수, 총알 개수
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


    //생존, 생존 수
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
        string("생존"),
        D3DCOLOR_XRGB(180, 180, 180),
        survivalBg
    );
    survival->SetDrawTextFormat(DT_CENTER);
    survival->SetPosition(D3DXVECTOR3(0.0f, 5.0f, 0.0f));

    auto survivalText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(26.0f, 26.0f),
        string("47"),
        D3DCOLOR_XRGB(255, 255, 255),
        m_background);
    survivalText->SetDrawTextFormat(DT_CENTER);
    survivalText->SetPosition(D3DXVECTOR3(1185.0f, 22.0f, 0.0f));

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