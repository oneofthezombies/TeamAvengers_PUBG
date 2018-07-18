#include "stdafx.h"
#include "Character.h"
#include "ScenePlay.h"

#include "UIImage.h"
#include "UIText.h"

Character::GameOverUI::GameOverUI()
    : IUIButtonOnMouseListener()
    , pPlayer(nullptr)

    , pBackgroundImg(nullptr)

    , pChickenText(nullptr)
    , pRankingNumText(nullptr)
    , pKillNumText(nullptr)
    , pUpRankingNumText(nullptr)
    , pUpPlayersNumText(nullptr)
{
}

Character::GameOverUI::~GameOverUI()
{
}

void Character::GameOverUI::OnMouseEnter()
{
}

void Character::GameOverUI::OnMouseExit()
{
}

void Character::GameOverUI::OnMouseDown(const int key)
{
}

void Character::GameOverUI::OnMouseUp(const int key)
{
    //cout << "UP" << endl;
}

void Character::GameOverUI::OnMouseDrag(const int key)
{
}

void Character::GameOverUI::Init(Character* pPlayer)
{
    this->pPlayer = pPlayer;

    ScenePlay* scenePlay = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
    UIObject* layer4 = scenePlay->GetLayer(4);

    // 검정 배경
    pBackgroundImg = new UIImage(
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
        pBackgroundImg);
    nickName->SetDrawTextFormat(DT_LEFT);
    nickName->SetPosition(D3DXVECTOR3(62.0f, 39.0f, 0.0f));

    pChickenText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverString),
        D3DXVECTOR2(947.0f, 50.0f),
        "그럴 수 있어. 이런 날도 있는 거지 뭐.", //이겼닭! 오늘 저녁은 치킨이닭!
        InGameUI::YELLOW,
        pBackgroundImg);
    pChickenText->SetDrawTextFormat(DT_LEFT);
    pChickenText->SetPosition(D3DXVECTOR3(62.0f, 97.0f, 0.0f));

    auto rankingBg = new UIImage(
        "./Resource/UI/GameOver/", 
        "ranking_bg.png",
        D3DXVECTOR3(59.95f, 174.67f, 0.0f),
        nullptr,
        pBackgroundImg);

    auto rankingText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(51.0f, 30.0f),
        "랭킹",
        InGameUI::WHITE,
        pBackgroundImg);
    rankingText->SetDrawTextFormat(DT_CENTER);
    rankingText->SetPosition(D3DXVECTOR3(61.95f, 174.69f, 0.0f));

    pRankingNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(50.0f, 30.0f),
        "#63",
        InGameUI::WHITE,
        pBackgroundImg);
    pRankingNumText->SetDrawTextFormat(DT_LEFT);
    pRankingNumText->SetPosition(D3DXVECTOR3(125.0f, 169.0f, 0.0f));

    auto killBg = new UIImage(
        "./Resource/UI/GameOver/",
        "kill_bg.png",
        D3DXVECTOR3(206.04f, 174.67f, 0.0f),
        nullptr,
        pBackgroundImg);

    auto killText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillText),
        D3DXVECTOR2(25.5f, 30.0f),
        "킬",
        InGameUI::WHITE,
        pBackgroundImg);
    killText->SetDrawTextFormat(DT_CENTER);
    killText->SetPosition(D3DXVECTOR3(207.04f, 174.69f, 0.0f));

    pKillNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverRankingAndKillNum),
        D3DXVECTOR2(27.0f, 30.0f),
        "0",
        InGameUI::WHITE,
        pBackgroundImg);
    pKillNumText->SetDrawTextFormat(DT_RIGHT);
    pKillNumText->SetPosition(D3DXVECTOR3(233.6f, 169.0f, 0.0f));

    auto playerText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverPlayerText),
        D3DXVECTOR2(65.0f, 18.0f),
        "플레이어",
        InGameUI::GRAY_ALPHA,
        pBackgroundImg);
    playerText->SetDrawTextFormat(DT_CENTER);
    playerText->SetPosition(D3DXVECTOR3(265.5f, 185.0f, 0.0f));

    pUpRankingNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverUpRanking),
        D3DXVECTOR2(90.0f, 58.0f),
        "#63",
        InGameUI::YELLOW,
        pBackgroundImg);
    pUpRankingNumText->SetDrawTextFormat(DT_LEFT);
    pUpRankingNumText->SetPosition(D3DXVECTOR3(1074.97f, 22.0f, 0.0f));

    pUpPlayersNumText = new UIText(
        Resource()()->GetFont(TAG_FONT::GameOverUpPlayersNum),
        D3DXVECTOR2(62.0f, 45.0f),
        "/97",
        InGameUI::GRAY_ALPHA,
        pBackgroundImg);
    pUpPlayersNumText->SetDrawTextFormat(DT_LEFT);
    pUpPlayersNumText->SetPosition(D3DXVECTOR3(1167.0f, 36.0f, 0.0f));


    UIButton* button = new UIButton(
        D3DXVECTOR3(58.5f, 623.0f, 0.0f),
        "./Resource/UI/GameOver/", 
        "lobby_btn_idle.png", 
        "lobby_btn_mouseover.png", 
        "lobby_btn_mouseover.png",
        this,
        pBackgroundImg,
        Resource()()->GetFont(TAG_FONT::GameOverLobby), "로비로 나가기",
        D3DCOLOR_XRGB(0, 0, 0));

  
    //하단은 꼴만 있고 기능 구현 x
    //관전
    new UIButton(
        D3DXVECTOR3(202.0f, 623.0f, 0.0f),
        "./Resource/UI/GameOver/",
        "watch_btn_idle.png",
        "watch_btn_mouseover.png",
        "watch_btn_mouseover.png",
        this,
        pBackgroundImg,
        Resource()()->GetFont(TAG_FONT::GameOverLobby), "",
        D3DCOLOR_XRGB(0, 0, 0));
    //신고
    new UIButton(
        D3DXVECTOR3(940.0f, 623.0f, 0.0f),
        "./Resource/UI/GameOver/",
        "report_btn_idle.png",
        "report_btn_mouseover.png",
        "report_btn_mouseover.png",
        this,
        pBackgroundImg,
        Resource()()->GetFont(TAG_FONT::GameOverLobby), "",
        D3DCOLOR_XRGB(0, 0, 0));
    //데스 캠
    new UIButton(
        D3DXVECTOR3(1083.0f, 623.0f, 0.0f),
        "./Resource/UI/GameOver/",
        "deathcam_btn_idle.png",
        "deathcam_btn_mouseover.png",
        "deathcam_btn_mouseover.png",
        this,
        pBackgroundImg,
        Resource()()->GetFont(TAG_FONT::GameOverLobby), "",
        D3DCOLOR_XRGB(0, 0, 0));

    pBackgroundImg->SetIsRender(false);
}

void Character::GameOverUI::Update()
{
    //TODO: 키보드 입력 막기, 마우스 자유롭게
    //TODO: 버그있음
    if (pPlayer->GetIsGameOver())
    {
        //남은 hp를 0으로 만든다
        pPlayer->GetInGameUI().SetRedToZero();
        
        //킬 수
        int killNum = pPlayer->GetKillNum();
        pKillNumText->SetText(to_string(killNum));

        //등수 = 생존 수 + 1
        ScenePlay* currentScene = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
        int survivors = currentScene->GetSurvivors();
        int rank = 0;
        if (survivors == 1)
        {
            rank = 1;
        }
        else
        {
            rank = survivors + 1;
        }
        pRankingNumText->SetText("#" + to_string(rank));
        pUpRankingNumText->SetText("#" + to_string(rank));

        //playersNum은 현재씬의 캐릭터 백터 사이즈 갖고오기
        int playersNum = static_cast<int>(currentScene->GetCharacters().size());
        pUpPlayersNumText->SetText("/" + to_string(playersNum));

        if (rank == 1)
        {
            pChickenText->SetText("이겼닭! 오늘 저녁은 치킨이닭!");
        }
        else
        {
            pChickenText->SetText("그럴 수 있어. 이런 날도 있는 거지 뭐.");
        }

        //값이 다 바뀌였으면 보여준다!
        pBackgroundImg->SetIsRender(true);
    }
}