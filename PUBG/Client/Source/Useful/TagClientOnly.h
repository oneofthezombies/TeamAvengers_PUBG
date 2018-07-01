#pragma once

enum class TAG_OBJECT
{
    Idle,
};

enum class TAG_COLLISION
{
    Idle,
    Impassable,
    Interactable,
    Player_0_Body,
    Player_1_Body,
    Player_2_Body,
    Player_3_Body,
    Player_0_Damage,
    Player_1_Damage,
    Player_2_Damage,
    Player_3_Damage,
};

/*

author : Team_Avengers
title : collider character part chart

https://docs.google.com/spreadsheets/d/1khCjEWCtUUwS7lJogOEOQPfJHVnhsOMDu277mzoVTQ4/edit#gid=951342423

*/

enum class TAG_COLLIDER_CHARACTER_PART
{
    Head,            // �Ӹ�
    Neck,            // ��
    Breast,          // ����
    Stomach_Upper,   // ��_��
    Stomach_Lower,   // ��_��
    Waist,           // �㸮
    Clavicle_Left,   // ���_��
    Clavicle_Right,  // ���_��
    Arm_Left_Upper,  // ��_�»�
    Arm_Left_Lower,  // ��_����
    Arm_Right_Upper, // ��_���
    Arm_Right_Lower, // ��_����
    Hand_Left,       // ��_��
    Hand_Right,      // ��_��
    Leg_Left_Upper,  // �ٸ�_�»�
    Leg_Left_Lower,  // �ٸ�_����
    Leg_Right_Upper, // �ٸ�_���
    Leg_Right_Lower, // �ٸ�_����
    Foot_Left,       // ��_��
    Foot_Right,      // ��_��
};

enum class TAG_SCENE
{
    Loading,
    Login,
    Play,
};

enum class TAG_FONT
{
    Default,
};

enum class TAG_SOUND
{
    Idle,
    Kar98_BoltMove0,
    Kar98_BoltMove1,
    Kar98_BoltMove2,
    Kar98_NormalShoot,
    Kar98_SilenceShoot,
    Kar98_Reload0,
    Kar98_Reload1,
    kar98_Reload2,
    kar98_Reload3,
};

enum class TAG_CHANNEL
{
    My_Ch,
    Other_Ch1,      //�ٸ� ���� �ϳ���.
    Other_Ch2,
    Other_Ch3,
    Environment_Ch
};

enum class TAG_CAMERA
{
    Default,
    First_Person,
    Third_Person,
    KyunChak,
    Scope2X
};
