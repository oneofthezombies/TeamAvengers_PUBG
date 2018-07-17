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

    // 검정 배경
    auto pBorder = new UIImage(
        "./Resource/UI/GameOver/",
        "black_1280_720_70.png",
        Vector3::ZERO,
        nullptr,
        layer4);
    //
    //pAmmoReloadText = new UIText(
    //    Resource()()->GetFont(TAG_FONT::InGameAmmoReload),
    //    D3DXVECTOR2(130.0f, 28.0f),
    //    string(""),
    //    InGameUI::WHITE,
    //    pAmmoBg);
    //pAmmoReloadText->SetDrawTextFormat(DT_CENTER);
    //pAmmoReloadText->SetPosition(D3DXVECTOR3(1.9f, 0.1f, 0.0f));
}

void Character::GameOverUI::Update()
{
}