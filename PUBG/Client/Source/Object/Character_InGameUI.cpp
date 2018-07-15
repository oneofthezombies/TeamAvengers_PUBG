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
const D3DCOLOR Character::InGameUI::WHITE_ALPHA = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
const D3DCOLOR Character::InGameUI::BLACK_ALPHA = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);

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

    , m_pBackground(nullptr)

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
    , INFO_TEXT_COOL_TIME(4.0f)
    , m_infoTextCoolDown(0.0f)

    , HP_COOL_TIME(1.0f)
    , m_hpCoolDown(0.0f)
{
}

Character::InGameUI::~InGameUI()
{
}

void Character::InGameUI::Destroy()
{
    UI()()->Destroy(m_pBackground);
}

void Character::InGameUI::Init(Character* pPlayer)
{
    this->pPlayer = pPlayer;

    m_infoTextCoolDown = INFO_TEXT_COOL_TIME;
    m_hpCoolDown = HP_COOL_TIME;

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
        D3DXVECTOR3(EQUIP_START, EQUIP_HEIGHT, 0.0f),
        nullptr,
        m_pBackground
    );

    pHelmetImg = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_helmet01.png",
        D3DXVECTOR3(EQUIP_START + EQUIP_WIDTH + EQUIP_GAP/*526.0f*/, EQUIP_HEIGHT, 0.0f),
        nullptr,
        m_pBackground
    );

    pVestImg = new UIImage(
        "./Resource/UI/InGame/",
        "equipment_vest01.png",
        D3DXVECTOR3(EQUIP_START + (EQUIP_WIDTH + EQUIP_GAP) * 2/*550.0f*/, EQUIP_HEIGHT, 0.0f),
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
    //pHpRedImg->SetSize(D3DXVECTOR2(255.0f, 17.0f));
    //pHpWhiteImg->SetSize(D3DXVECTOR2(250.0f, 17.0f));


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
        string(""),
        InGameUI::WHITE,
        ammoBg);
    pAmmoReloadText->SetDrawTextFormat(DT_CENTER);
    pAmmoReloadText->SetPosition(D3DXVECTOR3(1.9f, 0.1f, 0.0f));

    pAmmoNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameAmmoTotalNum),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        GRAY,
        ammoBg);
    pAmmoNumText->SetDrawTextFormat(DT_LEFT);
    pAmmoNumText->SetPosition(D3DXVECTOR3(95.0, 5.0f, 0.0f));

    //�߻���
    pFireModeText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameFireMode),
        D3DXVECTOR2(130.0f, 28.0f),
        string(""),
        GRAY,
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
        m_pBackground
    );

    pSurvivalNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameSurvivalNum),
        D3DXVECTOR2(26.0f, 26.0f),
        string(""),
        WHITE,
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
        GRAY,
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
        WHITE,
        killNumUpBg);
    pKillNumUpText->SetDrawTextFormat(DT_CENTER);
    pKillNumUpText->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));


    //���̵�, ���ӹ���
    pIdText = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameID),
        D3DXVECTOR2(87.0f, 9.0f),
        string("HelloWoori"),
        WHITE_ALPHA,
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
        RED,
        m_pBackground,
        D3DXVECTOR3(612.0f, 504.0f, 0.0f));

    setTextWithShadow(
        pKillText,
        pKillTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(400.0f, 20.0f),
        string("����� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"),
        WHITE,
        m_pBackground,
        D3DXVECTOR3(440.0f, 480.0f, 0.0f));

    //������ ��� �� �ȳ�����
    setTextWithShadow(
        pInfoText,
        pInfoTextShadow,
        Resource()()->GetFont(TAG_FONT::InGameInfo),
        D3DXVECTOR2(252.0f, 20.0f),
        string(""),
        WHITE,
        m_pBackground,
        D3DXVECTOR3(510.0f, 579.0f, 0.0f));

    //ų�α�
    pKillLog1 = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string("HelloWoori�� Kar98k(��)�� ���� Hoon��(��) ����߽��ϴ�"),
        GRAY,
        m_pBackground);
    pKillLog1->SetDrawTextFormat(DT_RIGHT);
    pKillLog1->SetPosition(D3DXVECTOR3(856.0f, 52.0f, 0.0f));

    pKillLog2  = new UIText(
        Resource()()->GetFont(TAG_FONT::InGameKillLog),
        D3DXVECTOR2(400.0f, 14.0f),
        string("John�� QBZ(��)�� ���� ootz��(��) ����߽��ϴ�"),
        GRAY,
        m_pBackground);
    pKillLog2->SetDrawTextFormat(DT_RIGHT);
    pKillLog2->SetPosition(D3DXVECTOR3(856.0f, 52.0f + 20.0f, 0.0f));

    //��
    auto primaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        PRIMARY_WEAPON_POS,
        nullptr,
        m_pBackground);

    auto secondaryWeaponBg = new UIImage(
        "./Resource/UI/InGame/",
        "weapons_bg.png",
        SECONDARY_WEAPON_POS,
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

    //��
    new UIImage(
        "./Resource/UI/InGame/",
        "map_bg.png",
        D3DXVECTOR3(1085.0f, 530.0f, 0.0f),
        nullptr,
        m_pBackground);
}

void Character::InGameUI::Update(const TotalInventory& inven)
{
    //���� ������ �޾Ƽ� UI�� ��������

    //�����ð��� ������ �ش� ���� ����
    updateInfoTextUI();

    //�տ� ��� �ִ� ���� ���������� �Ѿ��� ����
    updateOnHandWeaponUI(inven);

    //�����ڼ�
    updateSurvivalNumTextUI();

    //�� ���
    float hp = pPlayer->GetCharacterHealth();
    float hpWidth = HP_WIDTH / Character::MAX_HEALTH * hp;
    pHpWhiteImg->SetSize(D3DXVECTOR2(hpWidth, HP_HEIGHT));
    
    //1�� �� ���� hp�� �����ŭ �پ���
    if (pPlayer->IsDamaged())
    {
        float t = m_hpCoolDown -= Time()()->GetDeltaTime();
        if (m_hpCoolDown <= 0)
        {
            pHpRedImg->SetSize(D3DXVECTOR2(hpWidth, HP_HEIGHT));
            m_hpCoolDown = HP_COOL_TIME;
            pPlayer->ResetIsDamaged();
        }
    }

    //��� ���� ���� UI


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
        if (m_infoTextCoolDown <= 0)
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
            numBulletInInventory = (*it).second.back()->GetCount();
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