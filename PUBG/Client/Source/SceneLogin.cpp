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
        "./Resource_Temp/", "input_field.png", "input_field.png", "input_field.png", 
        TAG_FONT::DEFAULT, D3DCOLOR_XRGB(0, 0, 0), root);

    UIButton* button = new UIButton(D3DXVECTOR3(100.0f, 200.0f, 0.0f), 
        "./Resource_Temp/", "input_field.png", "input_field.png", "input_field.png", 
        &m_buttonListener, 
        root, 
        Resource()()->GetFont(TAG_FONT::DEFAULT), "Connect", 
        D3DCOLOR_XRGB(0, 0, 0));
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

    const auto c = Communication()();
    c->Connect(host, port, nickname);

    UI()()->Destroy(GetHandle());
    Scene()()->SetCurrentScene(TAG_SCENE::PLAY);
}

void LoginButtonListener::OnMouseDrag(const int key)
{
}
