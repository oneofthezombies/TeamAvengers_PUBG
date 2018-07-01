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
    Head,            // 머리
    Neck,            // 목
    Breast,          // 가슴
    Stomach_Upper,   // 배_상
    Stomach_Lower,   // 배_하
    Waist,           // 허리
    Clavicle_Left,   // 쇄골_좌
    Clavicle_Right,  // 쇄골_우
    Arm_Left_Upper,  // 팔_좌상
    Arm_Left_Lower,  // 팔_좌하
    Arm_Right_Upper, // 팔_우상
    Arm_Right_Lower, // 팔_우하
    Hand_Left,       // 손_좌
    Hand_Right,      // 손_우
    Leg_Left_Upper,  // 다리_좌상
    Leg_Left_Lower,  // 다리_좌하
    Leg_Right_Upper, // 다리_우상
    Leg_Right_Lower, // 다리_우하
    Foot_Left,       // 발_좌
    Foot_Right,      // 발_우
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
};

enum class TAG_CAMERA
{
    Default,
    First_Person,
    Third_Person,
    KyunChak,
    Scope2X
};

//enum class TAG_ITEM_CATEGORY
//{
//    Idle,
//    Equipment,  //장비
//    Consumable, //소모품
//    Ammo,       //탄약
//    Weapon,     //무기
//    Attach      //무기 부착물
//};

enum class TAG_ITEM_CATEGORY
{
    Idle,

    Armor,           //방탄조끼
    Back,            //백팩
    Head,            //헬멧

    Consumable,      //소모품
    Ammo,            //탄약

    Rifle,           //주무기/보조무기
    Melee,           //근접무기
    Throwable,       //수류탄류
    
    Attach           //무기 부착물
};

enum class TAG_ATTACH_POINT
{
    Primary_Muzzle,     //소음기, 소염기 등
    Primary_LowerRail,  //수직손잡이 등
    Primary_UpperRail,  //스코프류
    Primary_Mag,        //탄창
     
    Secondary_Muzzle,
    Secondary_LowerRail,
    Secondary_UpperRail,
    Secondary_Mag,
};