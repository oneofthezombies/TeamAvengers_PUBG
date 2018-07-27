#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ScenePlay.h"
#include "ComponentTransform.h"

//for UI
#include "UIImage.h"
#include "UIText.h"
#include "UICompass.h"

const D3DCOLOR Character::InGameUI::RED         = D3DCOLOR_XRGB(216, 0, 0);
const D3DCOLOR Character::InGameUI::WHITE       = D3DCOLOR_XRGB(255, 255, 255);
const D3DCOLOR Character::InGameUI::GRAY        = D3DCOLOR_XRGB(180, 180, 180);
const D3DCOLOR Character::InGameUI::YELLOW      = D3DCOLOR_XRGB(245, 193, 26);
const D3DCOLOR Character::InGameUI::WHITE_ALPHA = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
const D3DCOLOR Character::InGameUI::BLACK_ALPHA = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
const D3DCOLOR Character::InGameUI::GRAY_ALPHA  = D3DCOLOR_ARGB(100, 160, 160, 160);
const D3DCOLOR Character::InGameUI::GRAY_ALPHA2 = D3DCOLOR_ARGB(200, 248, 248, 248);

const D3DXVECTOR3 Character::InGameUI::PRIMARY_WEAPON_POS   = D3DXVECTOR3(970.0f, 670.0f, 0.0f);
const D3DXVECTOR3 Character::InGameUI::SECONDARY_WEAPON_POS = D3DXVECTOR3(970.0f, 632.0f, 0.0f);

const float Character::InGameUI::EQUIP_START  = 502.0f;
const float Character::InGameUI::EQUIP_WIDTH  = 21.0f;
const float Character::InGameUI::EQUIP_HEIGHT = 647.0f;
const float Character::InGameUI::EQUIP_GAP    = 3.0f;

const float Character::InGameUI::HP_WIDTH     = 276.0f;
const float Character::InGameUI::HP_HEIGHT    = 17.0f;

const float Character::InGameUI::AIM_BASE_X   = 640.0f;
const float Character::InGameUI::AIM_BASE_Y   = 360.0f;

const float Character::InGameUI::AIM_LEFT_X   = 640.0f - 11.9f - 7.0f;
const float Character::InGameUI::AIM_LEFT_Y   = 360.0f - 1.0f;

const float Character::InGameUI::AIM_RIGHT_X  = 640.0f + 11.9f;
const float Character::InGameUI::AIM_RIGHT_Y  = 360.0f - 1.0f;

const float Character::InGameUI::AIM_UP_X     = 640.0f - 1.0f;
const float Character::InGameUI::AIM_UP_Y     = 360.0f - 11.9f - 7.0f;

const float Character::InGameUI::AIM_DOWN_X   = 640.0f - 1.0f;
const float Character::InGameUI::AIM_DOWN_Y   = 360.0f + 11.9f;

Character::InGameUI::InGameUI()
    : pPlayer(nullptr)
    , m_nickName("")
    , m_killedNickName("")
    , m_weaponNameForKill("")

    , pBackground(nullptr)

    //Image ===================
    , pCompassBg(nullptr)
    , pCompass(nullptr)
    , pCompassArrowBg(nullptr)
    , pCompassArrow(nullptr)
    , pUICompass(nullptr)

    //, pBagImg(nullptr)
    //, pHelmetImg(nullptr)
    //, pVestImg(nullptr)

    , pHpRedImg(nullptr)
    , pHpWhiteImg(nullptr)

    , pAmmoBg(nullptr)

    , pQBZImg(nullptr)
    , pQBZRedImg(nullptr)
    , pKar98kImg(nullptr)
    , pKar98kRedImg(nullptr)

    , pKillTextUpBg(nullptr)
    , pKillNumUpBg(nullptr)

    , pMapImg(nullptr)

    , pAimCircle(nullptr)
    , pAimLeftLine(nullptr)
    , pAimRightLine(nullptr)
    , pAimUpLine(nullptr)
    , pAimDownLine(nullptr)
    , pInteractionBG(nullptr)
    , pInteractionF(nullptr)

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
    , pInteractionText(nullptr)

    //=========================
    , INFO_TEXT_COOL_TIME(4.0f)
    , m_infoTextCoolDown(0.0f)

    , HP_COOL_TIME(0.6f)
    , m_hpCoolDown(0.0f)

    , KILL_COOL_TIME(10.0f)
    , m_killCoolDown(0.0f)

    , KILL_UP_COOL_TIME(8.0f)
    , m_killUpCoolDown(0.0f)

    , m_isKill(false)

    , m_sumUp(0.0f)

    , BLOOD_COOL_TIME(1.0f)
    , m_bloodCoolDown(0.0f)
{
}

Character::InGameUI::~InGameUI()
{
}

void Character::InGameUI::Init(Character* pPlayer)
{
    this->pPlayer = pPlayer;

    m_infoTextCoolDown = INFO_TEXT_COOL_TIME;
    m_hpCoolDown = HP_COOL_TIME;
    m_killCoolDown = KILL_COOL_TIME;
    m_killUpCoolDown = KILL_UP_COOL_TIME;
    m_bloodCoolDown = BLOOD_COOL_TIME;

    ScenePlay* scenePlay = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    UIObject* layer2 = scenePlay->GetLayer(2);
    
    //투명 배경
    pBackground = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        layer2
    );
    
    //aim ======================================================
    pAimCircle = new UIImage(
        "./Resource/UI/InGame/",
        "aim_circle.png",
        D3DXVECTOR3(AIM_BASE_X - 1.0f, AIM_BASE_Y - 1.0f, 0.0f),
        nullptr,
        pBackground
    );
    pAimCircle->SetIsRender(false);

    //왼쪽 aim line
    pAimLeftLine = pAimLeftLine = new UIImage(
        "./Resource/UI/InGame/",
        "aim_hor.png",
        D3DXVECTOR3(AIM_LEFT_X, AIM_LEFT_Y, 0.0f),
        nullptr,
        pBackground
    );
    pAimLeftLine->SetIsRender(false);

    //오른쪽 aim line
    pAimRightLine = pAimUpLine = new UIImage(
        "./Resource/UI/InGame/",
        "aim_hor.png",
        D3DXVECTOR3(AIM_RIGHT_X, AIM_RIGHT_Y, 0.0f),
        nullptr,
        pBackground
    );
    pAimRightLine->SetIsRender(false);

    //위쪽 aim line
    pAimUpLine = new UIImage(
        "./Resource/UI/InGame/",
        "aim_ver.png",
        D3DXVECTOR3(AIM_UP_X, AIM_UP_Y, 0.0f),
        nullptr,
        pBackground
    );
    pAimUpLine->SetIsRender(false);

    //아래쪽 aim line
    pAimDownLine = new UIImage(
        "./Resource/UI/InGame/",
        "aim_ver.png",
        D3DXVECTOR3(AIM_DOWN_X, AIM_DOWN_Y, 0.0f),
        nullptr,
        pBackground
    );
    pAimDownLine->SetIsRender(false);
    //=========================================================


    //Compass 
    pCompassBg = new UIImage(
        "./Resource/UI/InGame/",
        "compass_bg.png",
        D3DXVECTOR3(420.0f, 28.0f, 0.0f),
        nullptr,
        pBackground
    );

    // removed 
    //pCompass = new UIImage(
    //    "./Resource/UI/InGame/",
    //    "compass.png",
    //    D3DXVECTOR3(-178.0f, 0.0f, 0.0f),
    //    nullptr,
    //    pCompassBg
    //);

    pCompassArrowBg = new UIImage(
        "./Resource/UI/InGame/",
        "compass_arraw_bg.png",
        D3DXVECTOR3(420.0f, 8.75f, 0.0f),
        nullptr,
        pBackground
    );

    pCompassArrow = new UIImage(
        "./Resource/UI/InGame/",
        "compass_arrow.png",
        D3DXVECTOR3(-178.0f, 0.0f, 0.0f),
        nullptr,
        pCompassArrowBg
    );

    pUICompass = new UICompass(pCompassBg);

    //for test
    //compassBg->SetIsRender(false);
    //부모를 가리면 자식도 함께 안보인다

    //장비템
    auto pEquip1 = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_panel.png",
        D3DXVECTOR3(EQUIP_START, EQUIP_HEIGHT, 0.0f),
        nullptr,
        pBackground
    );
    vecEquipImg.push_back(pEquip1);
    pEquip1->SetIsRender(false);

    auto pEquip2 = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_panel.png",
        D3DXVECTOR3(EQUIP_START + EQUIP_WIDTH + EQUIP_GAP/*526.0f*/, EQUIP_HEIGHT, 0.0f),
        nullptr,
        pBackground
    );
    vecEquipImg.push_back(pEquip2);
    pEquip2->SetIsRender(false);

    auto pEquip3 = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_panel.png",
        D3DXVECTOR3(EQUIP_START + (EQUIP_WIDTH + EQUIP_GAP) * 2/*550.0f*/, EQUIP_HEIGHT, 0.0f),
        nullptr,
        pBackground
    );
    vecEquipImg.push_back(pEquip3);
    pEquip3->SetIsRender(false);

    //hp
    auto hpBg = new UIImage(
        "./Resource/UI/InGame/",
        "player_HP_BG_v3.png",
        D3DXVECTOR3(502.0f, 684.0f, 0.0f),
        nullptr,
        pBackground);

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
    //pHpRedImg->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    //pHpWhiteImg->SetSize(D3DXVECTOR2(250.0f, 17.0f));

    //장전 수, 총알 개수
    pAmmoBg = new UIImage(
        "./Resource/UI/InGame/",
        "ammo_info.png",
        D3DXVECTOR3(576.0f, 647.0f, 0.0f),
        nullptr,
        pBackground);

    pAmmoReloadText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoReload),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        InGameUI::WHITE,
        pAmmoBg);
    pAmmoReloadText->SetDrawTextFormat(DT_CENTER);
    pAmmoReloadText->SetPosition(D3DXVECTOR3(1.9f, 0.1f, 0.0f));

    pAmmoNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoTotalNum),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        GRAY,
        pAmmoBg);
    pAmmoNumText->SetDrawTextFormat(DT_LEFT);
    pAmmoNumText->SetPosition(D3DXVECTOR3(95.0, 5.0f, 0.0f));

    //발사모드
    pFireModeText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameFireMode),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        GRAY,
        pAmmoBg);
    pFireModeText->SetDrawTextFormat(DT_LEFT);
    pFireModeText->SetPosition(D3DXVECTOR3(10.0, 8.0f, 0.0f));

    //생존, 생존 수
    auto survivalBg = new UIImage(
        "./Resource/UI/InGame/",
        "survival_text_bg.png",
        D3DXVECTOR3(1216.0f, 20.0f, 0.0f),
        nullptr,
        pBackground
    );

    auto survivalText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvival),
        D3DXVECTOR2(40.0f, 26.0f),
        string("생존"),
        GRAY,
        survivalBg
    );
    survivalText->SetDrawTextFormat(DT_CENTER);
    survivalText->SetPosition(D3DXVECTOR3(0.0f, 4.0f, 0.0f));

    auto survivalNumBg = new UIImage(
        "./Resource/UI/InGame/",
        "survival_num_bg.png",
        D3DXVECTOR3(1190.0f, 20.0f, 0.0f),
        nullptr,
        pBackground
    );

    pSurvivalNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(26.0f, 26.0f),
        string(""),
        WHITE,
        survivalNumBg);
    pSurvivalNumText->SetDrawTextFormat(DT_CENTER);
    pSurvivalNumText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    
    //여기
    //킬 (화면 오른쪽 상단)
    pKillTextUpBg = new UIImage(
        "./Resource/UI/InGame/",
        "kill_text_up_bg.png",
        D3DXVECTOR3(1145.0f, 20.0f, 0.0f),
        nullptr,
        pBackground
    );

    auto killUpText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvival),
        D3DXVECTOR2(24.0f, 26.0f),
        string("킬"),
        GRAY,
        pKillTextUpBg
    );
    killUpText->SetDrawTextFormat(DT_CENTER);
    killUpText->SetPosition(D3DXVECTOR3(0.0f, 4.0f, 0.0f));
    pKillTextUpBg->SetIsRender(false);

    pKillNumUpBg = new UIImage(
        "./Resource/UI/InGame/",
        "kill_num_up_bg.png",
        D3DXVECTOR3(1128.0f, 20.0f, 0.0f),
        nullptr,
        pBackground
    );

    pKillNumUpText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(17.0f, 26.0f),
        string(""),
        WHITE,
        pKillNumUpBg);
    pKillNumUpText->SetDrawTextFormat(DT_CENTER);
    pKillNumUpText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    pKillNumUpBg->SetIsRender(false);

    //아이디, 게임버전
    m_nickName = Communication()()->m_myInfo.nickname;
    pIdText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameID),
        D3DXVECTOR2(87.0f, 9.0f),
        m_nickName,
        WHITE_ALPHA,
        pBackground
    );
    pIdText->SetDrawTextFormat(DT_CENTER);
    pIdText->SetPosition(D3DXVECTOR3(598.0f, 705.0f, 0.0f));

    //킬 (게임 화면 중앙)
    setTextWithShadow(
        pKillNumText,
        pKillNumTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameKillNum),
        D3DXVECTOR2(60.0f, 30.0f),
        string(""),
        RED,
        pBackground,
        D3DXVECTOR3(612.0f, 504.0f, 0.0f));

    //ex) "당신의 Kar98k(으)로 인해 Hoon이(가) 사망했습니다"
    setTextWithShadow(
        pKillText,
        pKillTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(500.0f, 20.0f),
        string(""),
        WHITE,
        pBackground,
        D3DXVECTOR3(440.0f, 480.0f, 0.0f));

    //아이템 사용 등 안내문구
    setTextWithShadow(
        pInfoText,
        pInfoTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(252.0f, 20.0f),
        string(""),
        WHITE,
        pBackground,
        D3DXVECTOR3(510.0f, 579.0f, 0.0f));

    //킬로그
    //ex) "HelloWoori의 Kar98k(으)로 인해 Hoon이(가) 사망했습니다"
    auto pKillLog1 = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string(""),
        GRAY,
        pBackground);
    pKillLog1->SetDrawTextFormat(DT_RIGHT);
    pKillLog1->SetPosition(D3DXVECTOR3(856.0f, 52.0f, 0.0f));
    vecKillLog.push_back(pKillLog1);

    auto pKillLog2  = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string(""),
        GRAY,
        pBackground);
    pKillLog2->SetDrawTextFormat(DT_RIGHT);
    pKillLog2->SetPosition(D3DXVECTOR3(856.0f, 52.0f + 20.0f, 0.0f));
    vecKillLog.push_back(pKillLog2);

    auto pKillLog3 = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string(""),
        GRAY,
        pBackground);
    pKillLog3->SetDrawTextFormat(DT_RIGHT);
    pKillLog3->SetPosition(D3DXVECTOR3(856.0f, 52.0f + 40.0f, 0.0f));
    vecKillLog.push_back(pKillLog3);

    auto pKillLog4 = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string(""),
        GRAY,
        pBackground);
    pKillLog4->SetDrawTextFormat(DT_RIGHT);
    pKillLog4->SetPosition(D3DXVECTOR3(856.0f, 52.0f + 60.0f, 0.0f));
    vecKillLog.push_back(pKillLog4);

    //총
    pQBZImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_QBZ95.png",
        D3DXVECTOR3(PRIMARY_WEAPON_POS.x + 28.0f, PRIMARY_WEAPON_POS.y, PRIMARY_WEAPON_POS.z),
        nullptr,
        pBackground);
    pQBZImg->SetIsRender(false);
    
    pKar98kImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_kar98k.png",
        SECONDARY_WEAPON_POS,
        nullptr,
        pBackground);
    pKar98kImg->SetIsRender(false);

    pQBZRedImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_QBZ95_red.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        pQBZImg);
    pQBZRedImg->SetIsRender(false);

    pKar98kRedImg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_gun_kar98k_red.png",
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        nullptr,
        pKar98kImg);
    pKar98kRedImg->SetIsRender(false);

    //맵
    pMapImg = new UIImage(
        "./Resource/UI/InGame/",
        "map_bg.png",
        D3DXVECTOR3(1085.0f, 530.0f, 0.0f),
        nullptr,
        pBackground);

    //ui 피터지는.
    m_vecBlood.reserve(7);
    m_vecBlood.push_back(new UIImage(
        "./Resource/UI/InGame/Blood/",
        "b3.png",
        D3DXVECTOR3(416.0f, 347.0f, 0.0f), nullptr, pBackground));

    m_vecBlood.push_back(new UIImage(
        "./Resource/UI/InGame/Blood/",
        "b5.png",
        D3DXVECTOR3(0.0f, 425.0f, 0.0f), nullptr, pBackground));

    m_vecBlood.push_back(new UIImage(
        "./Resource/UI/InGame/Blood/",
        "br8.png",
        D3DXVECTOR3(984.0f, 396.0f, 0.0f), nullptr, pBackground));

    m_vecBlood.push_back((new UIImage(
        "./Resource/UI/InGame/Blood/",
        "b9.png",
        D3DXVECTOR3(30.0f, 20.0f, 0.0f), nullptr, pBackground)));

    m_vecBlood.push_back(new UIImage(
        "./Resource/UI/InGame/Blood/",
        "b13.png",
        D3DXVECTOR3(662.0f, 338.0f, 0.0f), nullptr, pBackground));

    m_vecBlood.push_back(new UIImage(
        "./Resource/UI/InGame/Blood/",
        "br14.png",
        D3DXVECTOR3(919.0f, 81.0f, 0.0f), nullptr, pBackground));

    m_vecBlood.push_back(new UIImage(
        "./Resource/UI/InGame/Blood/",
        "b15.png",
        D3DXVECTOR3(560.0f, 70.0f, 0.0f), nullptr, pBackground));

    for (auto a : m_vecBlood)
    {
        a->SetIsRender(false);
    }

    pInteractionF = new UIImage(
        "./Resource/UI/InGame/",
        "F.png",
        D3DXVECTOR3(713.0f, 404.0f, 0.0f),
        nullptr,
        pBackground
    );
    pInteractionF->SetIsRender(true);

    pInteractionBG = new UIImage(
        "./Resource/UI/InGame/",
        "F_name_bg.png",
        D3DXVECTOR3(27.0f, 0.0f, 0.0f),
        nullptr,
        pInteractionF
    );

    pInteractionText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(pInteractionBG->GetSize().x, pInteractionBG->GetSize().y),
        "TEST겸 글자 수 체크",
        WHITE_ALPHA,
        pInteractionBG
    );
    pInteractionText->SetDrawTextFormat(DT_CENTER);
    pInteractionText->SetPosition(D3DXVECTOR3(0.0f, 4.0f, 0.0f));
}

void Character::InGameUI::Update(const TotalInventory& inven)
{
    /*실제 정보를 받아서 UI를 변경하자*/

    //일정시간이 지나면 해당 문구 삭제
    updateInfoTextUI();

    //손에 들고 있는 총의 장전개수와 총알의 개수
    updateOnHandWeaponUI(inven);

    //생존자수
    updateSurvivalNumTextUI();

    //피 닳기
    updateHpUI();
    //피 이미지
    updateBloodUI();

    //TODO: 킬로그 (서버랑 연관해서 생각해야함), 동시에 들어왔을 때 변경
    if (pPlayer->GetIsKill())
    {
        //ex) "HelloWoori의 Kar98k(으)로 인해 Hoon이(가) 사망했습니다"
        string str = m_nickName + " 의"+ m_weaponNameForKill + "(으)로 인해 " + m_killedNickName + " 이(가) 사망했습니다";
        vecKillLog.at(0)->SetText(str);
    }

    //킬 수
    updateKillUI(inven);

    //장비 착용 관련 UI
    updateEquipUI(inven);

    //무기착용 UI
    updateWeaponUI(inven);

    // 컴퍼스 UI
    updateCompassUI();

    //aim
    if (inven.m_pHand && !inven.isOpened)
    {
        pAimCircle->SetIsRender(true);
        pAimLeftLine->SetIsRender(true);
        pAimRightLine->SetIsRender(true);
        pAimUpLine->SetIsRender(true);
        pAimDownLine->SetIsRender(true);
        
        //여기서부터
        //에임 벌어지는거 하면댐
        auto& backAction = pPlayer->GetWaitBackAction();
        if (backAction.Ing)
        {
            if (backAction.Up)
            {
                m_sumUp += backAction.curValX * 0.5f * 1000.0f;
                cout << "backAction: true" << endl;
                cout << "curValX_sumUp: " << backAction.curValX * 0.5f * 1000.0f << endl;
                cout << "sumDown: " << m_sumUp << endl;
                if (m_sumUp >= 50.0f)
                    m_sumUp = 50.0f;

                pAimLeftLine->SetPosition(D3DXVECTOR3(
                    AIM_LEFT_X - m_sumUp,
                    AIM_LEFT_Y,
                    0.0f));

                pAimRightLine->SetPosition(D3DXVECTOR3(
                    AIM_RIGHT_X + m_sumUp,
                    AIM_RIGHT_Y,
                    0.0f));

                pAimUpLine->SetPosition(D3DXVECTOR3(
                    AIM_UP_X,
                    AIM_UP_Y - m_sumUp,
                    0.0f));

                pAimDownLine->SetPosition(D3DXVECTOR3(
                    AIM_DOWN_X,
                    AIM_DOWN_Y + m_sumUp,
                    0.0f));
            }
            else //backAction.Up == false
            {
                pAimLeftLine->SetPosition(D3DXVECTOR3(
                    AIM_LEFT_X,
                    AIM_LEFT_Y,
                    0.0f));

                pAimRightLine->SetPosition(D3DXVECTOR3(
                    AIM_RIGHT_X,
                    AIM_RIGHT_Y,
                    0.0f));

                pAimUpLine->SetPosition(D3DXVECTOR3(
                    AIM_UP_X,
                    AIM_UP_Y,
                    0.0f));

                pAimDownLine->SetPosition(D3DXVECTOR3(
                    AIM_DOWN_X,
                    AIM_DOWN_Y,
                    0.0f));

                m_sumUp = 0.0f;
            }
        }
        else //backAction.Ing == false
        {
        }
    }
    else
    {
        pAimCircle->SetIsRender(false);
        pAimLeftLine->SetIsRender(false);
        pAimRightLine->SetIsRender(false);
        pAimUpLine->SetIsRender(false);
        pAimDownLine->SetIsRender(false);
    }
}

void Character::InGameUI::SetRedToZero()
{
    pHpRedImg->SetSize(D3DXVECTOR2(0.0f, InGameUI::HP_HEIGHT));
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
        BLACK_ALPHA,
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

void Character::InGameUI::updateInfoTextUI()
{
    if (pInfoText->GetText() != "")
    {
        float t = m_infoTextCoolDown -= Time()()->GetDeltaTime();
        //cout << t << endl;
        if (m_infoTextCoolDown <= 0.0f)
        {
            pInfoText->SetText("");
            pInfoTextShadow->SetText("");
            m_infoTextCoolDown = INFO_TEXT_COOL_TIME;
        }
    }
}

void Character::InGameUI::updateOnHandWeaponUI(const TotalInventory& inven)
{
    if (inven.m_pHand || inven.pTempSaveWeaponForX)
    {
        //총 교체시 UI 버그 fix용
        pQBZRedImg->SetIsRender(false);
        pKar98kRedImg->SetIsRender(false);

        TAG_RES_STATIC tag;
        TAG_RES_STATIC ammoType;

        if (inven.m_pHand)
        {
            tag = inven.m_pHand->GetTagResStatic(); //총 종류
            ammoType = ItemInfo::GetAmmoType(tag);  //탄약 종류
        }
        else if (inven.pTempSaveWeaponForX)
        {
            tag = inven.pTempSaveWeaponForX->GetTagResStatic(); //총 종류
            ammoType = ItemInfo::GetAmmoType(tag);  //탄약 종류
        }

        int numReloadBullet = 0;
        int numBulletInInventory = 0;

        //발사모드
        if (tag == TAG_RES_STATIC::Kar98k)
        {
            pFireModeText->SetText("단발");
        }
        else if (tag == TAG_RES_STATIC::QBZ)
        {
            if (inven.m_pHand)
            {
                if (inven.m_pHand->GetAuto())
                    pFireModeText->SetText("연사");
                else
                    pFireModeText->SetText("단발");
            }
            else if (inven.pTempSaveWeaponForX)
            {
                if (inven.pTempSaveWeaponForX->GetAuto())
                    pFireModeText->SetText("연사");
                else
                    pFireModeText->SetText("단발");
            }

        }

        //총에 장전된 총알 개수
        if (inven.m_pHand)
        {
            numReloadBullet = inven.m_pHand->GetNumBullet();
        }
        else if (inven.pTempSaveWeaponForX)
        {
            numReloadBullet = inven.pTempSaveWeaponForX->GetNumBullet();
        }

        //총알 개수
        auto it = inven.m_mapInventory.find(ammoType);
        if (it != inven.m_mapInventory.end())
        {
            if (!it->second.empty())
                numBulletInInventory = (*it).second.back()->GetCount();
            else
                numBulletInInventory = 0;
        }
        
        //장전이 안되어있다면 그림 & 텍스트 빨간색으로
        if (numReloadBullet == 0)
        {
            pAmmoReloadText->ChangeColor(RED);
            //TODO: 주무기, 보조무기에 따라 위치다르게 
            if (tag == TAG_RES_STATIC::Kar98k)
            {
                pKar98kRedImg->SetIsRender(true);
            }
            else if (tag == TAG_RES_STATIC::QBZ)
            {
                pQBZRedImg->SetIsRender(true);
            }
        }
        else
        {
            pAmmoReloadText->ChangeColor(WHITE);
            if (tag == TAG_RES_STATIC::Kar98k)
            {
                pKar98kRedImg->SetIsRender(false);
            }
            else if (tag == TAG_RES_STATIC::QBZ)
            {
                pQBZRedImg->SetIsRender(false);
            }
        }

        pAmmoNumText->SetText(to_string(numBulletInInventory));
        pAmmoReloadText->SetText(to_string(numReloadBullet));
    }
    else //if(m_pHand == nullptr)
    {
        pQBZRedImg->SetIsRender(false);
        pKar98kRedImg->SetIsRender(false);
        
        pFireModeText->SetText("");
        pAmmoReloadText->SetText("");
        pAmmoNumText->SetText("");
    }
}

void Character::InGameUI::updateSurvivalNumTextUI()
{
    ScenePlay* currentScene = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    int survivalNum = currentScene->GetSurvivors();
    pSurvivalNumText->SetText(to_string(survivalNum));
}

void Character::InGameUI::updateHpUI()
{
    float hp = pPlayer->GetCharacterHealth();
    float hpWidth = HP_WIDTH / Character::MAX_HEALTH * hp;
    pHpWhiteImg->SetSize(D3DXVECTOR2(hpWidth, HP_HEIGHT));

    //1초 뒤 빨간 hp도 흰색만큼 줄어든다
    if (pPlayer->IsDamaged())
    {
        m_hpCoolDown -= Time()()->GetDeltaTime();
        if (m_hpCoolDown <= 0.0f)
        {
            pHpRedImg->SetSize(D3DXVECTOR2(hpWidth, HP_HEIGHT));
            m_hpCoolDown = HP_COOL_TIME;
            //pPlayer->ResetIsDamaged();
        }
    }
}

void Character::InGameUI::updateKillUI(const TotalInventory& inven)
{
    if (inven.isOpened)
    {
        pKillNumUpBg->SetIsRender(true);
        pKillTextUpBg->SetIsRender(true);
        pKillNumUpText->SetText(to_string(pPlayer->GetKillNum()));

        pCompassBg->SetIsRender(false);
        pCompassArrowBg->SetIsRender(false);
        pAmmoBg->SetIsRender(false);
        pMapImg->SetIsRender(false);

        pQBZImg->SetIsRender(false);
        pKar98kImg->SetIsRender(false);
        pQBZRedImg->SetIsRender(false);
        pKar98kRedImg->SetIsRender(false);

    }
    else
    {
        if (m_isKill == false)
        {
            pKillNumUpBg->SetIsRender(false);
            pKillTextUpBg->SetIsRender(false);
            pKillNumUpText->SetText("");
        }

        pCompassBg->SetIsRender(true);
        pCompassArrowBg->SetIsRender(true);
        pAmmoBg->SetIsRender(true);
        pMapImg->SetIsRender(true);

        //pQBZImg->SetIsRender(true);
        //pKar98kImg->SetIsRender(true);
    }

    //킬 한 그 순간에 텍스트가 뜬다
    if (pPlayer->GetIsKill())
    {
        m_isKill = true;
        //화면 중앙 킬
        //ex) "당신의 Kar98k(으)로 인해 Hoon이(가) 사망했습니다"
        string str = string("당신의 ") + m_weaponNameForKill + "(으)로 인해 " + m_killedNickName + " 이(가) 사망했습니다";
        pKillText->SetText(str, pKillTextShadow);

        int killNum = pPlayer->GetKillNum();
        pKillNumText->SetText(to_string(killNum) + " 킬", pKillNumTextShadow);

        //화면 오른쪽 상단 킬
        pKillNumUpBg->SetIsRender(true);
        pKillTextUpBg->SetIsRender(true);
        pKillNumUpText->SetText(to_string(killNum));

        pPlayer->SetIsKill(false);
    }

    //일정시간이 지나면 텍스트가 사라진다
    //화면 중앙 킬
    if (pKillNumText->GetText() != "")
    {
        m_killCoolDown -= Time()()->GetDeltaTime();
        if (m_killCoolDown <= 0.0f)
        {
            pKillNumText->SetText("", pKillNumTextShadow);
            pKillText->SetText("", pKillTextShadow);

            m_killCoolDown = KILL_COOL_TIME;
        }
    }

    //화면 오른쪽 상단 킬
    if (pKillNumUpText->GetText() != "" && inven.isOpened == false)
    {
        float t = m_killUpCoolDown -= Time()()->GetDeltaTime();
        cout << t << endl;

        if (m_killUpCoolDown <= 0.0f)
        {
            m_isKill = false;

            pKillNumUpBg->SetIsRender(false);
            pKillTextUpBg->SetIsRender(false);
            pKillNumUpText->SetText("");

            m_killUpCoolDown = KILL_UP_COOL_TIME;
        }
    }
}

void Character::InGameUI::updateEquipUI(const TotalInventory& inven)
{
    if (pPlayer->GetIsEatEquip())
    {
        switch (inven.m_equipOnNum)
        {
        case 0:
        {
            vecEquipImg[0]->SetIsRender(false);
            vecEquipImg[1]->SetIsRender(false);
            vecEquipImg[2]->SetIsRender(false);
        }
        break;

        case 1:
        {
            if (inven.m_pEquipArmor)
            {
                vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_vest01.png");
                vecEquipImg[0]->SetIsRender(true);
            }
            else if (inven.m_pEquipBack)
            {
                vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_bag01.png");
                vecEquipImg[0]->SetIsRender(true);
            }
            else if (inven.m_pEquipHead)
            {
                vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_helmet01.png");
                vecEquipImg[0]->SetIsRender(true);
            }
        }
        break;

        case 2:
        {
            if (inven.m_pEquipArmor && inven.m_pEquipBack)
            {
                vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_bag01.png");
                vecEquipImg[1]->SetTexture("./Resource/UI/InGame/equipment_vest01.png");

                vecEquipImg[0]->SetIsRender(true);
                vecEquipImg[1]->SetIsRender(true);
            }
            else if (inven.m_pEquipArmor && inven.m_pEquipHead)
            {
                vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_helmet01.png");
                vecEquipImg[1]->SetTexture("./Resource/UI/InGame/equipment_vest01.png");

                vecEquipImg[0]->SetIsRender(true);
                vecEquipImg[1]->SetIsRender(true);
            }
            else if (inven.m_pEquipBack && inven.m_pEquipHead)
            {
                vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_bag01.png");
                vecEquipImg[1]->SetTexture("./Resource/UI/InGame/equipment_helmet01.png");

                vecEquipImg[0]->SetIsRender(true);
                vecEquipImg[1]->SetIsRender(true);
            }
        }
        break;

        case 3:
        {
            vecEquipImg[0]->SetTexture("./Resource/UI/InGame/equipment_bag01.png");
            vecEquipImg[1]->SetTexture("./Resource/UI/InGame/equipment_helmet01.png");
            vecEquipImg[2]->SetTexture("./Resource/UI/InGame/equipment_vest01.png");

            vecEquipImg[0]->SetIsRender(true);
            vecEquipImg[1]->SetIsRender(true);
            vecEquipImg[2]->SetIsRender(true);
        }
        break;

        default:
        {
            assert(false && "Character::InGameUI::updateEquipUI(), default case.");
        }
        break;
        }
        pPlayer->SetIsEatEquip(false);
    }
}

void Character::InGameUI::updateWeaponUI(const TotalInventory& inven)
{
    if (auto item = inven.m_pWeapon1->pItem)
    {
        if (!inven.isOpened)
        {
            //주무기
            TAG_RES_STATIC tag = item->GetTagResStatic();
            if (tag == TAG_RES_STATIC::Kar98k)
            {
                pKar98kImg->SetIsRender(true);
                pKar98kImg->SetPosition(PRIMARY_WEAPON_POS);
            }
            else if (tag == TAG_RES_STATIC::QBZ)
            {
                pQBZImg->SetIsRender(true);
                pQBZImg->SetPosition(D3DXVECTOR3(
                    PRIMARY_WEAPON_POS.x + 28.0f,
                    PRIMARY_WEAPON_POS.y,
                    PRIMARY_WEAPON_POS.z));
            }
        }
    }

    if (auto item = inven.m_pWeapon2->pItem)
    {
        if (!inven.isOpened)
        {
            //보조무기
            TAG_RES_STATIC tag = item->GetTagResStatic();
            if (tag == TAG_RES_STATIC::Kar98k)
            {
                pKar98kImg->SetIsRender(true);
                pKar98kImg->SetPosition(SECONDARY_WEAPON_POS);
            }
            else if (tag == TAG_RES_STATIC::QBZ)
            {
                pQBZImg->SetIsRender(true);
                pQBZImg->SetPosition(D3DXVECTOR3(
                    SECONDARY_WEAPON_POS.x + 28.0f,
                    SECONDARY_WEAPON_POS.y,
                    SECONDARY_WEAPON_POS.z));
            }
        }
    }
}

void Character::InGameUI::updateBloodUI()
{
    //디버그용
    //if(GetAsyncKeyState('Z')& 0x8000)
    //{
    //    this->pPlayer->MinusDamage(2);
    //    this->pPlayer->m_isDamaged = true;
    //}
    if (pPlayer->IsDamaged())
    {
        if (m_bloodCoolDown == BLOOD_COOL_TIME)
        {
            int randTemp;
            int countTemp=0;
            for (auto a : m_vecBlood)
            {
                randTemp = (rand() % 3)-1;
                if (randTemp>0 && countTemp <= 2)
                {
                    a->SetIsRender(true);
                    countTemp++;

                }
                
            }
        }
        m_bloodCoolDown -= Time()()->GetDeltaTime();
        for (auto a : m_vecBlood)
        {
            a->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_bloodCoolDown));
        }
        if (m_bloodCoolDown <= 0.0f)
        {
            for (auto a : m_vecBlood)
            {
                a->SetIsRender(false);
                m_bloodCoolDown = BLOOD_COOL_TIME;
                pPlayer->ResetIsDamaged();
            }
        }
    }
}

void Character::InGameUI::updateCompassUI()
{
    pUICompass->SetRotationY(pPlayer->GetForward());
}
