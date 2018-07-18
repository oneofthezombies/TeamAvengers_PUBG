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
    : IScene()
    , m_buttonListener()
{
}

SceneLogin::~SceneLogin()
{
}

void SceneLogin::OnInit()
{
    UIObject* root = new UIImage(
        "./Resource/", 
        "dedenne.png", 
        Vector3::ZERO, 
        nullptr, 
        nullptr);
    m_buttonListener.SetHandle(root);
    UI()()->RegisterUIObject(root);

    UITextInputField* inputField = new UITextInputField(
        D3DXVECTOR3(100.0f, 100.0f, 0.0f), 
        "./Resource/", "input_field.png", "input_field.png", "input_field.png", 
        TAG_FONT::Default, D3DCOLOR_XRGB(0, 0, 0), root);
    inputField->SetText("127.0.0.1 ");

    UIButton* button = new UIButton(D3DXVECTOR3(100.0f, 200.0f, 0.0f), 
        "./Resource/", "input_field.png", "input_field.png", "input_field.png", 
        &m_buttonListener, 
        root, 
        Resource()()->GetFont(TAG_FONT::Default), "Connect", 
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

    std::string host, port, nickname;
    ss >> host /*>> port*/ >> nickname;
    port = "8253";

    const auto c = Communication()();
    c->Connect(host, port, nickname);

    UI()()->Destroy(GetHandle());
    //Scene()()->SetCurrentScene(TAG_SCENE::Play);
    //for test
    Scene()()->SetCurrentScene(TAG_SCENE::Lobby);
}

void LoginButtonListener::OnMouseDrag(const int key)
{
}
