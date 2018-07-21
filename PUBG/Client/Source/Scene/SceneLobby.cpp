#include "stdafx.h"
#include "SceneLobby.h"
#include "UIImage.h"
#include "UIText.h"
#include "Quad.h"

const D3DXVECTOR3 SceneLobby::PLAYER_0_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 SceneLobby::PLAYER_1_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 SceneLobby::PLAYER_2_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 SceneLobby::PLAYER_3_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

const D3DXQUATERNION SceneLobby::PLAYER_0_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXQUATERNION SceneLobby::PLAYER_1_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXQUATERNION SceneLobby::PLAYER_2_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXQUATERNION SceneLobby::PLAYER_3_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

SceneLobby::SceneLobby()
    : IScene()
    , m_pBackground(nullptr)
    , m_readyButtonListener()
    , pQuad(nullptr)
{
}

SceneLobby::~SceneLobby()
{
}

void SceneLobby::OnInit()
{
    m_pBackground = new UIImage(
        "./Resource/UI/Lobby/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        nullptr
    );
    m_readyButtonListener.SetHandle(m_pBackground);
    UI()()->RegisterUIObject(m_pBackground);

    //PUBG logo
    new UIImage(
        "./Resource/UI/Lobby/",
        "pubg_white_small.png",
        D3DXVECTOR3(38.62f, 25.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    auto pMenuPlay = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenu),
        D3DXVECTOR2(100.0f, 100.0f),
        string("PLAY"),
        Character::InGameUI::YELLOW,
        m_pBackground);
    pMenuPlay->SetDrawTextFormat(DT_LEFT);
    pMenuPlay->SetPosition(D3DXVECTOR3(130.23f, 25.76f, 0.0f));

    auto pMenuCustomize = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenu),
        D3DXVECTOR2(100.0f, 100.0f),
        string("CUSTOMIZE"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    pMenuCustomize->SetDrawTextFormat(DT_LEFT);
    pMenuCustomize->SetPosition(D3DXVECTOR3(187.66f, 25.76f, 0.0f));

    auto pMenuStore = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenu),
        D3DXVECTOR2(100.0f, 100.0f),
        string("STORE"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    pMenuStore->SetDrawTextFormat(DT_LEFT);
    pMenuStore->SetPosition(D3DXVECTOR3(300.0f, 25.76f, 0.0f));

    auto pMenuStatistics = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenu),
        D3DXVECTOR2(120.0f, 100.0f),
        string("STATISTICS"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    pMenuStatistics->SetDrawTextFormat(DT_LEFT);
    pMenuStatistics->SetPosition(D3DXVECTOR3(371.51f, 25.76f, 0.0f));

    auto pMenuReplays = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenu),
        D3DXVECTOR2(100.0f, 100.0f),
        string("REPLAYS"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    pMenuReplays->SetDrawTextFormat(DT_LEFT);
    pMenuReplays->SetPosition(D3DXVECTOR3(486.06f, 25.76f, 0.0f));

    new UIImage(
        "./Resource/UI/Lobby/",
        "yellow_line.png",
        D3DXVECTOR3(129.33f, 55.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    auto pMenuSmallPublicMatch= new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenuSmall),
        D3DXVECTOR2(100.0f, 30.0f),
        string("PUBLIC MATCH"),
        Character::InGameUI::YELLOW,
        m_pBackground);
    pMenuSmallPublicMatch->SetDrawTextFormat(DT_LEFT);
    pMenuSmallPublicMatch->SetPosition(D3DXVECTOR3(129.33f, 63.0f, 0.0f));

    auto pMenuSmallCustomMatch = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenuSmall),
        D3DXVECTOR2(120.0f, 30.0f),
        string("CUSTOM MATCH"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    pMenuSmallCustomMatch->SetDrawTextFormat(DT_LEFT);
    pMenuSmallCustomMatch->SetPosition(D3DXVECTOR3(242.83f, 63.0f, 0.0f));

    //왼쪽 하단
    auto team = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyLeftSmall),
        D3DXVECTOR2(120.0f, 30.0f),
        string("TEAM"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    team->SetDrawTextFormat(DT_LEFT);
    team->SetPosition(D3DXVECTOR3(38.62f, 515.0f, 0.0f));

    auto teamBg = new UIImage(
        "./Resource/UI/Lobby/",
        "select_btn.png",
        D3DXVECTOR3(38.25f, 531.25f, 0.0f),
        nullptr,
        m_pBackground
    );

    auto teamText = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenuSmall),
        D3DXVECTOR2(120.0f, 30.0f),
        string("SOLO"),
        Character::InGameUI::YELLOW,
        teamBg);
    teamText->SetDrawTextFormat(DT_LEFT);
    teamText->SetPosition(D3DXVECTOR3(5.0f, 1.0f, 0.0f));

    auto perspective = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyLeftSmall),
        D3DXVECTOR2(120.0f, 30.0f),
        string("PERSPECTIVE"),
        Character::InGameUI::GRAY_ALPHA2,
        m_pBackground);
    perspective->SetDrawTextFormat(DT_LEFT);
    perspective->SetPosition(D3DXVECTOR3(38.62f, 562.0f, 0.0f));

    auto perspectiveBg = new UIImage(
        "./Resource/UI/Lobby/",
        "select_btn.png",
        D3DXVECTOR3(38.25f, 578.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    auto perspectiveText = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyMenuSmall),
        D3DXVECTOR2(120.0f, 30.0f),
        string("TPP"),
        Character::InGameUI::YELLOW,
        perspectiveBg);
    perspectiveText->SetDrawTextFormat(DT_LEFT);
    perspectiveText->SetPosition(D3DXVECTOR3(5.0f, 1.0f, 0.0f));

    //레디 버튼
    UIButton* button = new UIButton(D3DXVECTOR3(38.63f, 638.0f, 0.0f),
        "./Resource/UI/Lobby/", "ready_btn_idle.png", "ready_btn_mouseover.png", "ready_btn_mouseover.png",
        &m_readyButtonListener,
        m_pBackground,
        Resource()()->GetFont(TAG_FONT::LobbyReady), "READY",
        D3DCOLOR_XRGB(255, 255, 255));

    //설정
    new UIImage(
        "./Resource/UI/Lobby/",
        "setting.png",
        D3DXVECTOR3(1219.0f, 678.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    //프로필
    new UIImage(
        "./Resource/UI/Lobby/",
        "profile_bg.png",
        D3DXVECTOR3(1095.79f, 23.23f, 0.0f),
        nullptr,
        m_pBackground
    );

    //닉네임
    auto nickNameText = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyNickName),
        D3DXVECTOR2(200.0f, 30.0f),
        Communication()()->m_myInfo.nickname,
        Character::InGameUI::WHITE,
        m_pBackground);
    nickNameText->SetDrawTextFormat(DT_LEFT);
    nickNameText->SetPosition(D3DXVECTOR3(1132.5f, 24.0f, 0.0f));

    //레디 닉네임 (이런식으로 4명 캐릭터한테 붙이면됨)
    auto readyNickNameBg = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_nickname_bg.png",
        D3DXVECTOR3(656.0f, 356.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    new UIImage(
        "./Resource/UI/Lobby/",
        "ready_check.png",
        D3DXVECTOR3(5.0f, 2.0f, 0.0f),
        nullptr,
        readyNickNameBg
    );

    auto readyNickNameText = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyReadyNickName),
        D3DXVECTOR2(200.0f, 30.0f),
        "hellowoori",
        Character::InGameUI::WHITE,
        readyNickNameBg);
    readyNickNameText->SetDrawTextFormat(DT_LEFT);
    readyNickNameText->SetPosition(D3DXVECTOR3(22.0f, 3.0f, 0.0f));

    AddCharacters();

    pQuad = new Quad;
    pQuad->SetTexture(
        "./Resource/UI/Lobby/",
        "lobby_bg.png");
    AddObject(pQuad);
}

void SceneLobby::OnUpdate()
{
}

void SceneLobby::StartPlay()
{
    UI()()->Destroy(m_pBackground);
    RemoveCharacters();
    Scene()()->SetCurrentScene(TAG_SCENE::Play);
}

void SceneLobby::AddCharacters()
{
    for (auto c : Scene()()->GetCharacters())
        AddObject(c);
}

void SceneLobby::RemoveCharacters()
{
    for (auto c : Scene()()->GetCharacters())
        RemoveObject(c);
}

ReadyButtonListener::ReadyButtonListener()
    : m_isReady(false)
{
}

ReadyButtonListener::~ReadyButtonListener()
{
}

void ReadyButtonListener::OnMouseEnter()
{
}

void ReadyButtonListener::OnMouseExit()
{
}

void ReadyButtonListener::OnMouseDown(const int key)
{
}

void ReadyButtonListener::OnMouseUp(const int key)
{
    Sound()()->Play(TAG_SOUND::ButtonClick, Vector3::ZERO, 1.0f, FMOD_2D);

    m_isReady = !m_isReady;

    const int myID = Communication()()->m_myInfo.ID;
    if (m_isReady)
    {
        // TODO : 내 캐릭터의 레디UI 켜기
        Scene()()->GetPlayer()->SetReadyAnimation();
        Communication()()->SendIsReady(myID);
    }
    else
    {
        // TODO : 내 캐릭터의 레디UI 끄기
        Scene()()->GetPlayer()->SetNotReadyAnimation();
        Communication()()->SendIsNotReady(myID);
    }
}

void ReadyButtonListener::OnMouseDrag(const int key)
{
}
