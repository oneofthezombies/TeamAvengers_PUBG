#include "stdafx.h"
#include "Character.h"

#include "UIImage.h"

Character::InGameUI::InGameUI()
    : m_background(nullptr)
{
}

Character::InGameUI::~InGameUI()
{
}

void Character::InGameUI::Init()
{
    //투명 배경
    m_background = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        nullptr
    );
    UI()()->RegisterUIObject(m_background);

    //Compass - 사이즈 조정 필요
    //auto n = new UIImage(
    //    "./Resource/UI/InGame/",
    //    "compass.png",
    //    D3DXVECTOR3(40.0f, 22.0f, 0.0f),
    //    nullptr,
    //    m_background
    //);
    //n->SetIsRender(false);

    //부모를 가리면 자식도 함께 안보인다
}

void Character::InGameUI::Destroy()
{
    UI()()->Destroy(m_background);
}

void Character::InGameUI::Update()
{
}

void Character::InGameUI::Render()
{
}