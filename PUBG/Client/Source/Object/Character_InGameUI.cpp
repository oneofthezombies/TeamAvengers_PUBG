#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"

//for UI
#include "UIImage.h"
#include "UIText.h"

const D3DCOLOR Character::InGameUI::RED = D3DCOLOR_XRGB(216, 0, 0);
const D3DCOLOR Character::InGameUI::WHITE = D3DCOLOR_XRGB(255, 255, 255);
const D3DCOLOR Character::InGameUI::GRAY = D3DCOLOR_XRGB(180, 180, 180);
const D3DCOLOR Character::InGameUI::WHITE_ALPHA = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
const D3DCOLOR Character::InGameUI::BLACK_ALPHA = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);

const D3DXVECTOR3 Character::InGameUI::PRIMARY_WEAPON_POS = D3DXVECTOR3(970.0f, 670.0f, 0.0f);
const D3DXVECTOR3 Character::InGameUI::SECONDARY_WEAPON_POS = D3DXVECTOR3(970.0f, 632.0f, 0.0f);


Character::InGameUI::InGameUI()
    : m_pBackground(nullptr)

    //Image ===================
    , pCompass(nullptr)

    , pBagImg(nullptr)
    , pHelmetImg(nullptr)
    , pVestImg(nullptr)

    , pHpRedImg(nullptr)
    , pHpWhiteImg(nullptr)

    , pQBZImg(nullptr)
    , pQBZRedImg(nullptr)
    , pKar98kImg(nullptr)
    , pKar98kRedImg(nullptr)

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

    //투명 배경
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
    //부모를 가리면 자식도 함께 안보인다

    //장비템
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

    //피 닳기 코드
    pHpRedImg->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    pHpWhiteImg->SetSize(D3DXVECTOR2(250.0f, 17.0f));


    //장전 수, 총알 개수
    auto ammoBg = new UIImage(
        "./Resource/UI/InGame/",
        "ammo_info.png",
        D3DXVECTOR3(576.0f, 647.0f, 0.0f),
        nullptr,
        m_pBackground);

    pAmmoReloadText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoReload),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        InGameUI::WHITE,
        ammoBg);
    pAmmoReloadText->SetDrawTextFormat(DT_CENTER);
    pAmmoReloadText->SetPosition(D3DXVECTOR3(1.9f, 0.1f, 0.0f));

    pAmmoNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoTotalNum),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        InGameUI::GRAY,
        ammoBg);
    pAmmoNumText->SetDrawTextFormat(DT_LEFT);
    pAmmoNumText->SetPosition(D3DXVECTOR3(95.0, 5.0f, 0.0f));

    //발사모드
    pFireModeText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameFireMode),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        InGameUI::GRAY,
        ammoBg);
    pFireModeText->SetDrawTextFormat(DT_LEFT);
    pFireModeText->SetPosition(D3DXVECTOR3(10.0, 8.0f, 0.0f));

    //생존, 생존 수
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
        string("생존"),
        InGameUI::GRAY,
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
        InGameUI::WHITE,
        survivalNumBg);
    pSurvivalNumText->SetDrawTextFormat(DT_CENTER);
    pSurvivalNumText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    
    //킬 (화면 오른쪽 상단)
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
        string("킬"),
        InGameUI::GRAY,
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
        InGameUI::WHITE,
        killNumUpBg);
    pKillNumUpText->SetDrawTextFormat(DT_CENTER);
    pKillNumUpText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));


    //아이디, 게임버전
    pIdText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameID),
        D3DXVECTOR2(87.0f, 9.0f),
        string("HelloWoori"),
        InGameUI::WHITE_ALPHA,
        m_pBackground
    );
    pIdText->SetDrawTextFormat(DT_CENTER);
    pIdText->SetPosition(D3DXVECTOR3(598.0f, 705.0f, 0.0f));

    //킬 (게임 화면 중앙)
    setTextWithShadow(
        pKillNumText,
        pKillNumTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameKillNum),
        D3DXVECTOR2(60.0f, 30.0f),
        string("2 킬"),
        InGameUI::RED,
        m_pBackground,
        D3DXVECTOR3(612.0f, 504.0f, 0.0f));

    setTextWithShadow(
        pKillText,
        pKillTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(400.0f, 20.0f),
        string("당신의 Kar98k(으)로 인해 Hoon이(가) 사망했습니다"),
        InGameUI::WHITE,
        m_pBackground,
        D3DXVECTOR3(440.0f, 480.0f, 0.0f));

    //아이템 사용 등 안내문구
    setTextWithShadow(
        pInfoText,
        pInfoTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(252.0f, 20.0f),
        string(""),
        InGameUI::WHITE,
        m_pBackground,
        D3DXVECTOR3(510.0f, 579.0f, 0.0f));

    //킬로그
    pKillLog1 = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string("HelloWoori의 Kar98k(으)로 인해 Hoon이(가) 사망했습니다"),
        InGameUI::GRAY,
        m_pBackground);
    pKillLog1->SetDrawTextFormat(DT_RIGHT);
    pKillLog1->SetPosition(D3DXVECTOR3(856.0f, 52.0f, 0.0f));

    pKillLog2  = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string("John의 QBZ(으)로 인해 ootz이(가) 사망했습니다"),
        InGameUI::GRAY,
        m_pBackground);
    pKillLog2->SetDrawTextFormat(DT_RIGHT);
    pKillLog2->SetPosition(D3DXVECTOR3(856.0f, 52.0f + 20.0f, 0.0f));

    //총
    auto primaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        InGameUI::PRIMARY_WEAPON_POS,
        nullptr,
        m_pBackground);

    auto secondaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        InGameUI::SECONDARY_WEAPON_POS,
        nullptr,
        m_pBackground);

    pQBZImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_QBZ95.png",
        D3DXVECTOR3(28.0f, 0.0f, 0.0f),
        nullptr,
        primaryWeaponBg);

    pKar98kImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_kar98k.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        secondaryWeaponBg);

    pQBZRedImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_QBZ95_red.png",
        D3DXVECTOR3(28.0f, 0.0f, 0.0f),
        nullptr,
        primaryWeaponBg);
    pQBZRedImg->SetIsRender(false);

    pKar98kRedImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_kar98k_red.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        secondaryWeaponBg);
    pKar98kRedImg->SetIsRender(false);

    //맵
    new UIImage(
        "./Resource/UI/InGame/",
        "map_bg.png",
        D3DXVECTOR3(1085.0f, 530.0f, 0.0f),
        nullptr,
        m_pBackground);
}

void Character::InGameUI::Update(const TotalInventory& inven)
{
    //실제 정보를 받아서 string을 넣어주자
    //for test
    pKillNumUpText->SetText(string("9"));

    //일정시간이 지나면 해당 문구 삭제
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

    //손에 들고 있는 총의 장전개수와 총알의 개수
    if (inven.m_pHand)
    {
        TAG_RES_STATIC tag = inven.m_pHand->GetTagResStatic(); //총 종류
        TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag);  //탄약 종류

        int numReloadBullet = 0;
        int numBulletInInventory = 0;

        //발사모드
        if (tag == TAG_RES_STATIC::Kar98k)
        {
            pFireModeText->SetText("단발");
        }
        else if (tag == TAG_RES_STATIC::QBZ)
        {
            if (inven.m_pHand->GetAuto())
                pFireModeText->SetText("연사");
            else
                pFireModeText->SetText("단발");
        }

        //총에 장전된 총알 개수
        numReloadBullet = inven.m_pHand->GetNumBullet();

        //총알 개수
        auto it = inven.m_mapInventory.find(ammoType);
        if (it != inven.m_mapInventory.end())
        {
            numBulletInInventory = (*it).second.back()->GetCount();
        }

        //장전이 안되어있다면 빨간색으로
        if (numReloadBullet == 0)
        {
            pAmmoReloadText->ChangeColor(InGameUI::RED);
            //TODO: 주무기, 보조무기에 따라 위치다르게 
            if (tag == TAG_RES_STATIC::Kar98k)
            {
                pQBZRedImg->SetIsRender(true);
            }
            else if (tag == TAG_RES_STATIC::QBZ)
            {
                pKar98kRedImg->SetIsRender(true);
            }
        }
        else
        {
            pAmmoReloadText->ChangeColor(InGameUI::WHITE);
            if (tag == TAG_RES_STATIC::Kar98k)
            {
                pQBZRedImg->SetIsRender(false);
            }
            else if (tag == TAG_RES_STATIC::QBZ)
            {
                pKar98kRedImg->SetIsRender(false);
            }
        }

        pAmmoNumText->SetText(to_string(numBulletInInventory));
        pAmmoReloadText->SetText(to_string(numReloadBullet));
    }
    else //if(m_pHand == nullptr)
    {
        pFireModeText->SetText("");
        pAmmoReloadText->SetText("");
        pAmmoNumText->SetText("");
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
        InGameUI::BLACK_ALPHA,
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