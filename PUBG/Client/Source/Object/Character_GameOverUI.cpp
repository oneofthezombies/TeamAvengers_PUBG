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

    // °ËÁ¤ ¹è°æ
    auto pBackground = new UIImage(
        "./Resource/UI/GameOver/",
        "black_1280_720_70.png",
        Vector3::ZERO,
        nullptr,
        layer4);
    
    auto nickName = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverNickName),
        D3DXVECTOR2(300.0f, 33.0f),
        Communication()()->m_myInfo.nickname,
        InGameUI::WHITE,
        pBackground);
    nickName->SetDrawTextFormat(DT_LEFT);
    nickName->SetPosition(D3DXVECTOR3(62.0f, 39.0f, 0.0f));

    auto str = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverString),
        D3DXVECTOR2(947.0f, 50.0f),
        "±×·² ¼ö ÀÖ¾î. ÀÌ·± ³¯µµ ÀÖ´Â °ÅÁö ¹¹.", //ÀÌ°å´ß! ¿À´Ã Àú³áÀº Ä¡Å²ÀÌ´ß!
        D3DCOLOR_XRGB(237, 192, 27),
        pBackground);
    str->SetDrawTextFormat(DT_LEFT);
    str->SetPosition(D3DXVECTOR3(62.0f, 97.0f, 0.0f));

    auto rankingBg = new UIImage(
        "./Resource/UI/GameOver/", 
        "ranking_bg.png",
        D3DXVECTOR3(59.95f, 174.67f, 0.0f),
        nullptr,
        pBackground);

    auto rankingText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(51.0f, 30.0f),
        "·©Å·",
        InGameUI::WHITE,
        pBackground);
    rankingText->SetDrawTextFormat(DT_CENTER);
    rankingText->SetPosition(D3DXVECTOR3(61.95f, 174.69f, 0.0f));

    auto rankingNum = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(50.0f, 30.0f),
        "#63",
        InGameUI::WHITE,
        pBackground);
    rankingNum->SetDrawTextFormat(DT_LEFT);
    rankingNum->SetPosition(D3DXVECTOR3(125.0f, 169.0f, 0.0f));

    auto killBg = new UIImage(
        "./Resource/UI/GameOver/",
        "kill_bg.png",
        D3DXVECTOR3(206.04f, 174.67f, 0.0f),
        nullptr,
        pBackground);

    auto killText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(25.5f, 30.0f),
        "Å³",
        InGameUI::WHITE,
        pBackground);
    killText->SetDrawTextFormat(DT_CENTER);
    killText->SetPosition(D3DXVECTOR3(207.04f, 174.69f, 0.0f));

    auto killNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(27.0f, 30.0f),
        "0",
        InGameUI::WHITE,
        pBackground);
    killNumText->SetDrawTextFormat(DT_RIGHT);
    killNumText->SetPosition(D3DXVECTOR3(233.6f, 169.0f, 0.0f));

    auto playerText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverPlayerText),
        D3DXVECTOR2(65.0f, 18.0f),
        "ÇÃ·¹ÀÌ¾î",
        D3DCOLOR_ARGB(100, 160, 160, 160),
        pBackground);
    playerText->SetDrawTextFormat(DT_CENTER);
    playerText->SetPosition(D3DXVECTOR3(265.5f, 185.0f, 0.0f));

    auto upRanking = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverUpRanking),
        D3DXVECTOR2(90.0f, 58.0f),
        "#63",
        D3DCOLOR_XRGB(237, 192, 27),
        pBackground);
    upRanking->SetDrawTextFormat(DT_LEFT);
    upRanking->SetPosition(D3DXVECTOR3(1074.97f, 22.0f, 0.0f));

    auto upPlayersNum = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverUpPlayersNum),
        D3DXVECTOR2(62.0f, 45.0f),
        "/97",
        D3DCOLOR_ARGB(100, 160, 160, 160),
        pBackground);
    upPlayersNum->SetDrawTextFormat(DT_LEFT);
    upPlayersNum->SetPosition(D3DXVECTOR3(1167.0f, 36.0f, 0.0f));

    auto lobbyBtn = new UIImage(
        "./Resource/UI/GameOver/",
        "lobby_btn_idle.png",
        D3DXVECTOR3(1083.0f, 623.0f, 0.0f),
        nullptr,
        pBackground);

    auto lobbyText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverLobby),
        D3DXVECTOR2(120.0f, 25.0f),
        "·Îºñ·Î ³ª°¡±â",
        D3DCOLOR_XRGB(0, 0, 0),
        lobbyBtn);
    lobbyText->SetDrawTextFormat(DT_CENTER | DT_VCENTER);
    lobbyText->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void Character::GameOverUI::Update()
{
}