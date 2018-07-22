#include "stdafx.h"
#include "SceneLobby.h"
#include "UIImage.h"
#include "UIText.h"
#include "Quad.h"
#include "Character.h"
#include "ComponentTransform.h"

const D3DXVECTOR3 SceneLobby::PLAYER_0_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 SceneLobby::PLAYER_1_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 SceneLobby::PLAYER_2_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 SceneLobby::PLAYER_3_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

const D3DXQUATERNION SceneLobby::PLAYER_0_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXQUATERNION SceneLobby::PLAYER_1_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXQUATERNION SceneLobby::PLAYER_2_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXQUATERNION SceneLobby::PLAYER_3_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

SceneLobby::SceneLobby()
    : IScene(TAG_SCENE::Lobby)
    , m_pBackground(nullptr)
    , m_readyButtonListener()
    , pQuad(nullptr)
{
    for (auto& i : m_isReadys)
        i = false;

    for (auto& i : m_IDs)
        i = -1;
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

    //For Ready ============================================ 

    //닉네임 배경
    m_forReady[0].pBackground = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_nickname_bg.png",
        D3DXVECTOR3(442.0f, 312.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    m_forReady[1].pBackground = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_nickname_bg.png",
        D3DXVECTOR3(820.0f, 341.0f, 0.0f),
        nullptr,
        m_pBackground
    );
    
    m_forReady[2].pBackground = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_nickname_bg.png",
        D3DXVECTOR3(663.0f, 335.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    m_forReady[3].pBackground = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_nickname_bg.png",
        D3DXVECTOR3(970.0f, 345.0f, 0.0f),
        nullptr,
        m_pBackground
    );

    //레디 체크 UI
    m_forReady[0].pReadyCheck = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_check.png",
        D3DXVECTOR3(62.0f, 2.0f, 0.0f),
        nullptr,
        m_forReady[0].pBackground
    );

    m_forReady[1].pReadyCheck = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_check.png",
        D3DXVECTOR3(62.0f, 2.0f, 0.0f),
        nullptr,
        m_forReady[1].pBackground
    );

    m_forReady[2].pReadyCheck = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_check.png",
        D3DXVECTOR3(62.0f, 2.0f, 0.0f),
        nullptr,
        m_forReady[2].pBackground
    );

    m_forReady[3].pReadyCheck = new UIImage(
        "./Resource/UI/Lobby/",
        "ready_check.png",
        D3DXVECTOR3(62.0f, 2.0f, 0.0f),
        nullptr,
        m_forReady[3].pBackground
    );

    //캐릭터 닉네임
    m_forReady[0].pNickname = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyReadyNickName),
        D3DXVECTOR2(200.0f, 30.0f),
        "p1",
        Character::InGameUI::WHITE,
        m_forReady[0].pBackground);
    m_forReady[0].pNickname->SetDrawTextFormat(DT_LEFT);
    m_forReady[0].pNickname->SetPosition(D3DXVECTOR3(6.0f, 3.0f, 0.0f));

    m_forReady[1].pNickname = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyReadyNickName),
        D3DXVECTOR2(200.0f, 30.0f),
        "p2",
        Character::InGameUI::WHITE,
        m_forReady[1].pBackground);
    m_forReady[1].pNickname->SetDrawTextFormat(DT_LEFT);
    m_forReady[1].pNickname->SetPosition(D3DXVECTOR3(6.0f, 3.0f, 0.0f));

    m_forReady[2].pNickname = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyReadyNickName),
        D3DXVECTOR2(200.0f, 30.0f),
        "p3",
        Character::InGameUI::WHITE,
        m_forReady[2].pBackground);
    m_forReady[2].pNickname->SetDrawTextFormat(DT_LEFT);
    m_forReady[2].pNickname->SetPosition(D3DXVECTOR3(6.0f, 3.0f, 0.0f));

    m_forReady[3].pNickname = new UIText(
        Resource()()->GetFont(TAG_FONT::LobbyReadyNickName),
        D3DXVECTOR2(200.0f, 30.0f),
        "p4",
        Character::InGameUI::WHITE,
        m_forReady[3].pBackground);
    m_forReady[3].pNickname->SetDrawTextFormat(DT_LEFT);
    m_forReady[3].pNickname->SetPosition(D3DXVECTOR3(6.0f, 3.0f, 0.0f));

    //캐릭터가 들어와야 보이게
    m_forReady[0].pBackground->SetIsRender(false);
    m_forReady[1].pBackground->SetIsRender(false);
    m_forReady[2].pBackground->SetIsRender(false);
    m_forReady[3].pBackground->SetIsRender(false);

    //캐릭터가 레디를 해야 보이게
    m_forReady[0].pReadyCheck->SetIsRender(false);
    m_forReady[1].pReadyCheck->SetIsRender(false);
    m_forReady[2].pReadyCheck->SetIsRender(false);
    m_forReady[3].pReadyCheck->SetIsRender(false);

    //============================================ For Ready

    AddCharacters();

    pQuad = new Quad;
    pQuad->SetTexture(
        "./Resource/UI/Lobby/",
        "lobby_bg.png");
    pQuad->SetPosition(D3DXVECTOR3(970.5f, 146.5f, 193.0f));
    pQuad->SetScale(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
    AddObject(pQuad);

    const std::vector<Character*>& characters = Scene()()->GetCharacters();

    //characters[0]->GetTransform()->SetPosition(D3DXVECTOR3(899.0f, 48.0f, 0.0f));
    //characters[0]->GetTransform()->Update();
    //characters[1]->GetTransform()->SetPosition(D3DXVECTOR3(1076.0f, 40.5f, 35.0f));
    //characters[1]->GetTransform()->Update();
    //characters[2]->GetTransform()->SetPosition(D3DXVECTOR3(1011.0f, 36.0f, 114.0f));
    //characters[2]->GetTransform()->Update();
    //characters[3]->GetTransform()->SetPosition(D3DXVECTOR3(1216.5f, 38.0f, 75.0f));
    //characters[3]->GetTransform()->Update();

    Light()()->SetPositionInTargetSpace(D3DXVECTOR3(-200.0f, 200.0f, -100.0f));
    Light()()->SetTarget(characters[0]->GetTransform());
}

void SceneLobby::OnUpdate()
{
    const std::vector<Character*>& characters = Scene()()->GetCharacters();
    auto& pis = Communication()()->m_roomInfo.playerInfos;

    for (std::size_t i = 0; i < pis.size(); i++)
    {
        if (pis[i].ID < 0) continue;

        bool& prev = m_isReadys[i];
        bool  curr = pis[i].isReady;
        
        if (prev == false && 
            curr == true)
        {
            m_forReady[i].pReadyCheck->SetIsRender(true);
            characters[i]->SetReadyAnimation();
        }

        if (prev == true && 
            curr == false)
        {
            m_forReady[i].pReadyCheck->SetIsRender(false);
            characters[i]->SetNotReadyAnimation();
        }

        prev = curr;
    }

    for (std::size_t i = 0; i < pis.size(); i++)
    {
        int& prev = m_IDs[i];
        int  curr = pis[i].ID;

        if (prev < 0 && 
            curr >= 0)
        {
            m_forReady[i].pNickname->SetText(pis[i].nickname);
            m_forReady[i].pBackground->SetIsRender(true);

            switch (i)
            {
            case 0:
                {
                    characters[0]->GetTransform()->SetPosition(D3DXVECTOR3(899.0f, 48.0f, 0.0f));
                }
                break;
            case 1:
                {
                    characters[1]->GetTransform()->SetPosition(D3DXVECTOR3(1076.0f, 40.5f, 35.0f));
                }
                break;
            case 2:
                {
                    characters[2]->GetTransform()->SetPosition(D3DXVECTOR3(1011.0f, 36.0f, 114.0f));
                }
                break;
            case 3:
                {
                    characters[3]->GetTransform()->SetPosition(D3DXVECTOR3(1216.5f, 38.0f, 75.0f));
                }
                break;
            }

            characters[i]->GetTransform()->Update();
        }

        prev = curr;
    }
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

    const int myID = Communication()()->m_myInfo.ID;
    bool& myIsReady = Communication()()->m_roomInfo.playerInfos[myID].isReady;

    myIsReady = !myIsReady;

    if (myIsReady)
    {
        Communication()()->SendIsReady(myID);
    }
    else
    {
        Communication()()->SendIsNotReady(myID);
    }
}

void ReadyButtonListener::OnMouseDrag(const int key)
{
}
