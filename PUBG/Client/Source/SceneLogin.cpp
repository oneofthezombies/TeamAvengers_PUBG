#include "stdafx.h"
#include "SceneLogin.h"
#include "UITextInputField.h"
#include "Bandage.h"
#include "Kar98k.h"

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
    //UIObject* root = new UIObject(nullptr);
    //m_buttonListener.SetHandle(root);

    //UITextInputField* inputField = new UITextInputField(D3DXVECTOR3(100.0f, 100.0f, 0.0f), 
    //    "./Resource/", "input_field.png", "input_field.png", "input_field.png", 
    //    TAG_FONT::DEFAULT, D3DCOLOR_XRGB(0, 0, 0), root);

    //UIButton* button = new UIButton(D3DXVECTOR3(100.0f, 200.0f, 0.0f), 
    //    "./Resource/", "input_field.png", "input_field.png", "input_field.png", 
    //    &m_buttonListener, 
    //    root, 
    //    g_pFontManager->GetFont(TAG_FONT::DEFAULT), "Connect", D3DCOLOR_XRGB(0, 0, 0));

    IObject* p = nullptr;
    p = new Kar98k; AddObject(p);
    p = new Bandage; AddObject(p);
    //Sleep(500);
    //p = new Kar98k; AddObject(p);
}

void SceneLogin::OnUpdate()
{

}

void SceneLogin::OnRender()
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

    const auto ti = static_cast<UITextInputField*>(GetHandle()->GetChild(CHILD::UITEXT_INPUT_FIELD));
    const auto text = ti->GetText();

    stringstream ss(text);

    char host[32];
    char port[32];
    char nickname[32];
    ss >> host >> port >> nickname;

    cout << "host : " << host << ", port : " << port << ", nickname : " << nickname << endl;

    const auto com = g_pCommunicator;
    const auto r = com->Connect(host, port, nickname);
    if (r)
    {
        com->info.myNickname = nickname;
        g_pUIManager->Destroy(GetHandle());
        g_pSceneManager->SetCurrentScene(TAG_SCENE::PLAY);
    }
    else
    {
        cout << "connection failed.\n";
    }
}

void LoginButtonListener::OnMouseDrag(const int key)
{
}
