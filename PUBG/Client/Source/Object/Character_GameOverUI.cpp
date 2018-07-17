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

    //auto rankingBg = new UIImage(
    //    "./Resource/UI/InGame/",
    //    "ranking_bg.png",
    //    D3DXVECTOR3(59.0f, 170.0f, 0.0f),
    //    nullptr,
    //    pBackground);

    auto rankingText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(51.0f, 30.0f),
        "랭킹",
        InGameUI::WHITE,
        pBackground);
    rankingText->SetDrawTextFormat(DT_LEFT);
    rankingText->SetPosition(D3DXVECTOR3(62.0f, 170.0f, 0.0f));

    auto rankingNum = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(43.0f, 24.0f),
        "#63",
        InGameUI::WHITE,
        pBackground);
    rankingNum->SetDrawTextFormat(DT_LEFT);
    rankingNum->SetPosition(D3DXVECTOR3(128.75f, 173.75f, 0.0f));

    //auto killBg = new UIImage(
    //    "./Resource/UI/InGame/",
    //    "kill_bg.png",
    //    D3DXVECTOR3(245.0f, 170.0f, 0.0f),
    //    nullptr,
    //    pBackground);

    auto killText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(25.0f, 30.0f),
        "킬",
        InGameUI::WHITE,
        pBackground);
    killText->SetDrawTextFormat(DT_LEFT);
    killText->SetPosition(D3DXVECTOR3(249.25f, 170.0f, 0.0f));

    auto killNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(27.0f, 24.0f),
        "0",
        InGameUI::WHITE,
        pBackground);
    killNumText->SetDrawTextFormat(DT_LEFT);
    killNumText->SetPosition(D3DXVECTOR3(276.25f, 172.7f, 0.0f));

    auto playerText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverPlayerText),
        D3DXVECTOR2(65.0f, 16.0f),
        "플레이어",
        D3DCOLOR_XRGB(160, 160, 160),
        pBackground);
    playerText->SetDrawTextFormat(DT_LEFT);
    playerText->SetPosition(D3DXVECTOR3(308.25f, 182.0f, 0.0f));

    auto upRanking = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverUpRanking),
        D3DXVECTOR2(46.0f, 90.0f),
        "#63",
        D3DCOLOR_XRGB(237, 192, 27),
        pBackground);
    upRanking->SetDrawTextFormat(DT_LEFT);
    upRanking->SetPosition(D3DXVECTOR3(1064.68f, 28.0f, 0.0f));

    auto upPlayersNum = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverUpPlayersNum),
        D3DXVECTOR2(60.0f, 40.0f),
        "/97",
        D3DCOLOR_XRGB(160, 160, 160),
        pBackground);
    upPlayersNum->SetDrawTextFormat(DT_LEFT);
    upPlayersNum->SetPosition(D3DXVECTOR3(1158.84f, 34.75f, 0.0f));

    auto lobbyBtn = new UIImage(
        "./Resource/UI/InGame/",
        "lobby_btn_idle.png",
        D3DXVECTOR3(1083.0f, 623.0f, 0.0f),
        nullptr,
        pBackground);

    auto lobbyText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverLobby),
        D3DXVECTOR2(120.0f, 25.0f),
        "로비로 나가기",
        D3DCOLOR_XRGB(0, 0, 0),
        lobbyBtn);
    lobbyText->SetDrawTextFormat(DT_LEFT);
    lobbyText->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void Character::GameOverUI::Update()
{
}