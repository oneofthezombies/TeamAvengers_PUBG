#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ScenePlay.h"

//for UI
#include "UIImage.h"
#include "UIText.h"

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

    ScenePlay* scenePlay = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    UIObject* layer2 = scenePlay->GetLayer(2);
    
    //���� ���
    pBackground = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        layer2
    );

    //Compass
    pCompassBg = new UIImage(
        "./Resource/UI/InGame/",
        "compass_bg.png",
        D3DXVECTOR3(420.0f, 28.0f, 0.0f),
        nullptr,
        pBackground
    );

    pCompass = new UIImage(
        "./Resource/UI/InGame/",
        "compass.png",
        D3DXVECTOR3(-178.0f, 0.0f, 0.0f),
        nullptr,
        pCompassBg
    );

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

    //for test
    //compassBg->SetIsRender(false);
    //�θ� ������ �ڽĵ� �Բ� �Ⱥ��δ�

    //�����
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

    //�� ��� �ڵ�
    //pHpRedImg->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    //pHpWhiteImg->SetSize(D3DXVECTOR2(250.0f, 17.0f));

    //���� ��, �Ѿ� ����
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

    //�߻���
    pFireModeText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameFireMode),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        GRAY,
        pAmmoBg);
    pFireModeText->SetDrawTextFormat(DT_LEFT);
    pFireModeText->SetPosition(D3DXVECTOR3(10.0, 8.0f, 0.0f));

    //����, ���� ��
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
        string("����"),
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
    
    //����
    //ų (ȭ�� ������ ���)
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
        string("ų"),
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

    //���̵�, ���ӹ���
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

    //ų (���� ȭ�� �߾�)
    setTextWithShadow(
        pKillNumText,
        pKillNumTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameKillNum),
        D3DXVECTOR2(60.0f, 30.0f),
        string(""),
        RED,
        pBackground,
        D3DXVECTOR3(612.0f, 504.0f, 0.0f));

    //ex) "����� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"
    setTextWithShadow(
        pKillText,
        pKillTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(500.0f, 20.0f),
        string(""),
        WHITE,
        pBackground,
        D3DXVECTOR3(440.0f, 480.0f, 0.0f));

    //������ ��� �� �ȳ�����
    setTextWithShadow(
        pInfoText,
        pInfoTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(252.0f, 20.0f),
        string(""),
        WHITE,
        pBackground,
        D3DXVECTOR3(510.0f, 579.0f, 0.0f));

    //ų�α�
    //ex) "HelloWoori�� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"
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

    //��
    auto primaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        PRIMARY_WEAPON_POS,
        nullptr,
        pBackground);

    auto secondaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        SECONDARY_WEAPON_POS,
        nullptr,
        pBackground);

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

    //��
    pMapImg = new UIImage(
        "./Resource/UI/InGame/",
        "map_bg.png",
        D3DXVECTOR3(1085.0f, 530.0f, 0.0f),
        nullptr,
        pBackground);
}

void Character::InGameUI::Update(const TotalInventory& inven)
{
    /*���� ������ �޾Ƽ� UI�� ��������*/

    //�����ð��� ������ �ش� ���� ����
    updateInfoTextUI();

    //�տ� ��� �ִ� ���� ���������� �Ѿ��� ����
    updateOnHandWeaponUI(inven);

    //�����ڼ�
    updateSurvivalNumTextUI();

    //�� ���
    updateHpUI();

    //TODO: ų�α� (������ �����ؼ� �����ؾ���), ���ÿ� ������ �� ����
    if (pPlayer->GetIsKill())
    {
        //ex) "HelloWoori�� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"
        string str = m_nickName + " ��"+ m_weaponNameForKill + "(��)�� ���� " + m_killedNickName + " ��(��) ����߽��ϴ�";
        vecKillLog.at(0)->SetText(str);
    }

    //ų ��
    updateKillUI(inven);

    //��� ���� ���� UI
    updateEquipUI(inven);
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
    if (inven.m_pHand)
    {
        //�� ��ü�� UI ���� fix��
        pQBZRedImg->SetIsRender(false);
        pKar98kRedImg->SetIsRender(false);

        TAG_RES_STATIC tag = inven.m_pHand->GetTagResStatic(); //�� ����
        TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag);  //ź�� ����

        int numReloadBullet = 0;
        int numBulletInInventory = 0;

        //�߻���
        if (tag == TAG_RES_STATIC::Kar98k)
        {
            pFireModeText->SetText("�ܹ�");
        }
        else if (tag == TAG_RES_STATIC::QBZ)
        {
            if (inven.m_pHand->GetAuto())
                pFireModeText->SetText("����");
            else
                pFireModeText->SetText("�ܹ�");
        }

        //�ѿ� ������ �Ѿ� ����
        numReloadBullet = inven.m_pHand->GetNumBullet();

        //�Ѿ� ����
        auto it = inven.m_mapInventory.find(ammoType);
        if (it != inven.m_mapInventory.end())
        {
            if (!it->second.empty())
                numBulletInInventory = (*it).second.back()->GetCount();
            else
                numBulletInInventory = 0;
        }
        
        //������ �ȵǾ��ִٸ� �׸� & �ؽ�Ʈ ����������
        if (numReloadBullet == 0)
        {
            pAmmoReloadText->ChangeColor(RED);
            //TODO: �ֹ���, �������⿡ ���� ��ġ�ٸ��� 
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

    //1�� �� ���� hp�� �����ŭ �پ���
    if (pPlayer->IsDamaged())
    {
        m_hpCoolDown -= Time()()->GetDeltaTime();
        if (m_hpCoolDown <= 0.0f)
        {
            pHpRedImg->SetSize(D3DXVECTOR2(hpWidth, HP_HEIGHT));
            m_hpCoolDown = HP_COOL_TIME;
            pPlayer->ResetIsDamaged();
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

        pQBZImg->SetIsRender(true);
        pKar98kImg->SetIsRender(true);
    }

    //ų �� �� ������ �ؽ�Ʈ�� ���
    if (pPlayer->GetIsKill())
    {
        m_isKill = true;
        //ȭ�� �߾� ų
        //ex) "����� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"
        string str = string("����� ") + m_weaponNameForKill + "(��)�� ���� " + m_killedNickName + " ��(��) ����߽��ϴ�";
        pKillText->SetText(str, pKillTextShadow);

        int killNum = pPlayer->GetKillNum();
        pKillNumText->SetText(to_string(killNum) + " ų", pKillNumTextShadow);

        //ȭ�� ������ ��� ų
        pKillNumUpBg->SetIsRender(true);
        pKillTextUpBg->SetIsRender(true);
        pKillNumUpText->SetText(to_string(killNum));

        pPlayer->SetIsKill(false);
    }

    //�����ð��� ������ �ؽ�Ʈ�� �������
    //ȭ�� �߾� ų
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

    //ȭ�� ������ ��� ų
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