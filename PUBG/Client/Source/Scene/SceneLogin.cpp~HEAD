#include "stdafx.h"
#include "SceneLogin.h"
#include "UITextInputField.h"
#include "UIImage.h"

enum CHILD
{
    UITEXT_INPUT_FIELD,
    UIBUTTON,
};

SceneLogin::SceneLogin()
    : IScene(TAG_SCENE::Login)
    , m_buttonListener()
{
}

SceneLogin::~SceneLogin()
{
}

void SceneLogin::OnInit()
{
    UIObject* root = new UIImage(
        "./Resource/UI/Login/", 
        "login_bg.png", 
        Vector3::ZERO, 
        nullptr, 
        nullptr);
    m_buttonListener.SetHandle(root);
    UI()()->RegisterUIObject(root);

    UITextInputField* inputField = new UITextInputField(
<<<<<<< HEAD:PUBG/Client/Source/SceneLogin.cpp
        D3DXVECTOR3(100.0f, 100.0f, 0.0f), 
        "./Resource_Temp/", "input_field.png", "input_field.png", "input_field.png", 
        TAG_FONT::DEFAULT, D3DCOLOR_XRGB(0, 0, 0), root);

    UIButton* button = new UIButton(D3DXVECTOR3(100.0f, 200.0f, 0.0f), 
        "./Resource_Temp/", "input_field.png", "input_field.png", "input_field.png", 
=======
        D3DXVECTOR3(386.0f, 325.5f, 0.0f), 
        "./Resource/UI/Login/", "input_idle.png", "input_mouseover.png", "input_mouseover.png", 
        TAG_FONT::LoginInput, D3DCOLOR_XRGB(255, 255, 255), root);
    inputField->SetText("127.0.0.1 ");

    UIButton* button = new UIButton(D3DXVECTOR3(580.13f, 419.42f, 0.0f), 
        "./Resource/UI/Login/", "login_btn_idle.png", "login_btn_mouseover.png", "login_btn_mouseover.png", 
>>>>>>> for_recording:PUBG/Client/Source/Scene/SceneLogin.cpp
        &m_buttonListener, 
        root, 
        Resource()()->GetFont(TAG_FONT::LoginButton), "CONNECT",
        D3DCOLOR_XRGB(255, 255, 255));
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
    Sound()()->Play(TAG_SOUND::ButtonClick, Vector3::ZERO, 1.0f, FMOD_2D);
}

void LoginButtonListener::OnMouseUp(const int key)
{
    cout << "Pushed\n";

    const auto ti = static_cast<UITextInputField*>(
        GetHandle()->GetChild(CHILD::UITEXT_INPUT_FIELD));
    const auto text = ti->GetText();

    stringstream ss(text);

    std::string host, port, nickname;
    ss >> host /*>> port*/ >> nickname;
    port = "8253";

    const auto c = Communication()();
    c->Connect(host, port, nickname);

    UI()()->Destroy(GetHandle());
    Scene()()->SetupCharacters();
    Scene()()->SetCurrentScene(TAG_SCENE::Lobby);
}

void LoginButtonListener::OnMouseDrag(const int key)
{
}
