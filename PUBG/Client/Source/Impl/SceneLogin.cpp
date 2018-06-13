#include "stdafx.h"
#include "SceneLogin.h"
#include "UITextInputField.h"

SceneLogin::SceneLogin()
    : IScene()
{
}

SceneLogin::~SceneLogin()
{
}

void SceneLogin::OnInit()
{
    UITextInputField* inputField = new UITextInputField(D3DXVECTOR3(100.0f, 100.0f, 0.0f), "./Resource/", "input_field.png", "input_field.png", "input_field.png", TAG_FONT::DEFAULT);
    g_pUIManager->RegisterUIObject(inputField);
}

void SceneLogin::OnUpdate()
{
}

void SceneLogin::OnRender()
{
}
