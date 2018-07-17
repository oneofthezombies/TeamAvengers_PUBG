#include "stdafx.h"
#include "Character.h"
#include "ScenePlay.h"

#include "UIImage.h"
#include "UIText.h"

Character::GameOverUI::GameOverUI()
    : pPlayer(nullptr)
{
}

Character::GameOverUI::~GameOverUI()
{
}

void Character::GameOverUI::Init(Character * pPlayer)
{
    this->pPlayer = pPlayer;

    ScenePlay* scenePlay = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    UIObject* layer4 = scenePlay->GetLayer(4);

    // 검정 배경
    auto pBackground = new UIImage(
        "./Resource/UI/GameOver/",
        "black_1280_720_70.png",
        Vector3::ZERO,
        nullptr,
        layer4);
    
    auto nickName = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverNickName),
        D3DXVECTOR2(300.0f, 33.0f),
        pPlayer->GetNickName(),
        InGameUI::WHITE,
        pBackground);
    nickName->SetDrawTextFormat(DT_LEFT);
    nickName->SetPosition(D3DXVECTOR3(62.0f, 34.0f, 0.0f));

    auto str = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverString),
        D3DXVECTOR2(947.0f, 50.0f),
        "그럴 수 있어. 이런 날도 있는 거지 뭐.",
        D3DCOLOR_XRGB(237, 192, 27),
        pBackground);
    str->SetDrawTextFormat(DT_LEFT);
    str->SetPosition(D3DXVECTOR3(62.0f, 92.0f, 0.0f));

    auto rankingBg = new UIImage(
        "./Resource/UI/InGame/",
        "ranking_bg.png",
        D3DXVECTOR3(59.0f, 170.0f, 0.0f),
        nullptr,
        pBackground);

    auto rankingText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(90.0f, 30.0f),
        "랭킹",
        InGameUI::WHITE,
        rankingBg);
    rankingText->SetDrawTextFormat(DT_LEFT);
    rankingText->SetPosition(D3DXVECTOR3(3.0f, 0.0f, 0.0f));

    auto rankingNum = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(43.0f, 24.0f),
        "#63",
        InGameUI::WHITE,
        rankingBg);
    rankingNum->SetDrawTextFormat(DT_LEFT);
    rankingNum->SetPosition(D3DXVECTOR3(104.0f, 3.0f, 0.0f));

    auto killBg = new UIImage(
        "./Resource/UI/InGame/",
        "kill_bg.png",
        D3DXVECTOR3(245.0f, 170.0f, 0.0f),
        nullptr,
        pBackground);

    auto killText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(25.0f, 30.0f),
        "킬",
        InGameUI::WHITE,
        killBg);
    killText->SetDrawTextFormat(DT_LEFT);
    killText->SetPosition(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
}

void Character::GameOverUI::Update()
{
}