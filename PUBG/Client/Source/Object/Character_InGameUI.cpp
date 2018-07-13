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
    //���� ���
    m_background = new UIImage(
        "./Resource/UI/InGame/",
        "transparent_1280_720",
        Vector3::ZERO,
        nullptr,
        nullptr
    );
    UI()()->RegisterUIObject(m_background);

    //Compass - ������ ���� �ʿ�
    //auto n = new UIImage(
    //    "./Resource/UI/InGame/",
    //    "compass.png",
    //    D3DXVECTOR3(40.0f, 22.0f, 0.0f),
    //    nullptr,
    //    m_background
    //);
    //n->SetIsRender(false);

    //�θ� ������ �ڽĵ� �Բ� �Ⱥ��δ�
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