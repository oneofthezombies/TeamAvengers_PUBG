#include "stdafx.h"
#include "SceneLogin.h"
#include "UITextInputField.h"

enum CHILD
{
    UITEXT_INPUT_FIELD,
    UIBUTTON,
};

SceneLogin::SceneLogin()
    : IScene()
    , m_buttonListener()
{
}

SceneLogin::~SceneLogin()
{
}

void SceneLogin::OnInit()
{
    UIObject* root = new UIObject(nullptr);
    m_buttonListener.SetHandle(root);

    UITextInputField* inputField = new UITextInputField(
        D3DXVECTOR3(100.0f, 100.0f, 0.0f), 
        "./Resource/", "input_field.png", "input_field.png", "input_field.png", 
        TAG_FONT::DEFAULT, D3DCOLOR_XRGB(0, 0, 0), root);

    UIButton* button = new UIButton(D3DXVECTOR3(100.0f, 200.0f, 0.0f), 
        "./Resource/", "input_field.png", "input_field.png", "input_field.png", 
        &m_buttonListener, 
        root, 
        g_pFontManager->GetFont(TAG_FONT::DEFAULT), "Connect", 
        D3DCOLOR_XRGB(0, 0, 0));

    g_pCollisionManager->SubscribeCollisionEvent(
        TAG_COLLISION::DAMAGE_OF_PLAYER_2, TAG_COLLISION::BODY_OF_PLAYER_1);
}

void SceneLogin::OnUpdate()
{
}

LoginButtonListener::LoginButtonListener()
{
}

LoginButtonListener::~LoginButtonListener()
{
}

void LoginButtonListener::OnMouseEnter()
{
}

void LoginButtonListener::OnMouseExit()
{
}

void LoginButtonListener::OnMouseDown(const int key)
{
}

void LoginButtonListener::OnMouseUp(const int key)
{
    cout << "Pushed\n";

    const auto ti = static_cast<UITextInputField*>(
        GetHandle()->GetChild(CHILD::UITEXT_INPUT_FIELD));
    const auto text = ti->GetText();

    stringstream ss(text);

    string host, port, nickname;
    ss >> host >> port >> nickname;

    const auto c = g_pCommunicator;
    c->Connect(host, port, nickname);

    g_pUIManager->Destroy(GetHandle());
    g_pSceneManager->SetCurrentScene(TAG_SCENE::PLAY);
}

void LoginButtonListener::OnMouseDrag(const int key)
{
}
