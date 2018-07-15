#pragma once

enum class TAG_OBJECT
{
    Idle,
    TerrainFeature,
    Bullet,
    Character,
    Door,
    Window,
    Item,
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
    Head,            // ¸Ó¸®
    Neck,            // ¸ñ
    Breast,          // °¡½¿
    Stomach_Upper,   // ¹è_»ó
    Stomach_Lower,   // ¹è_ÇÏ
    Waist,           // Çã¸®
    Clavicle_Left,   // ¼â°ñ_ÁÂ
    Clavicle_Right,  // ¼â°ñ_¿ì
    Arm_Left_Upper,  // ÆÈ_ÁÂ»ó
    Arm_Left_Lower,  // ÆÈ_ÁÂÇÏ
    Arm_Right_Upper, // ÆÈ_¿ì»ó
    Arm_Right_Lower, // ÆÈ_¿ìÇÏ
    Hand_Left,       // ¼Õ_ÁÂ
    Hand_Right,      // ¼Õ_¿ì
    Leg_Left_Upper,  // ´Ù¸®_ÁÂ»ó
    Leg_Left_Lower,  // ´Ù¸®_ÁÂÇÏ
    Leg_Right_Upper, // ´Ù¸®_¿ì»ó
    Leg_Right_Lower, // ´Ù¸®_¿ìÇÏ
    Foot_Left,       // ¹ß_ÁÂ
    Foot_Right,      // ¹ß_¿ì
    COUNT
};

enum class TAG_SCENE
{
    Loading,
    Login,
    Play,
    CollisionTest,
};

enum class TAG_FONT
{
    Default,
    Invetory_Ground,
    Invetory_28,
    InGameAmmoReload,
    InGameAmmoTotalNum,
    InGameSurvival,
    InGameSurvivalNum,
    InGameID,
    InGameKillNum,
    InGameKillLog,
    InGameInfo,
    InGameFireMode,
};

enum class TAG_SOUND
{
    Idle,

    Kar98_NormalShoot,
    Qbz_NormalShoot,

    Kar98_SilenceShoot,
    Qbz_SilenceShoot,


    Kar98_BoltMove0,
    Kar98_BoltMove1,
    Kar98_BoltMove2,
    Kar98_Reload0,
    Kar98_Reload1,
    Kar98_Reload2,
    Kar98_Reload3,
    Qbz_BoltMove76,
    Qbz_BoltMove77,
    Qbz_FakeShoot,
    Qbz_Reload,

    Background,
};

enum class TAG_CHANNEL
{
    My_Ch,
    Other_Ch1,      //´Ù¸¥ À¯Àú ÇÏ³ª´ç.
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
    Scope2X,
    OnGun
};

enum class TAG_ITEM_CATEGORY
{
    Idle,

    Armor,           //¹æÅºÁ¶³¢
    Back,            //¹éÆÑ
    Head,            //Çï¸ä

    Consumable,      //¼Ò¸ðÇ°
    Ammo,            //Åº¾à

    Rifle,           //ÁÖ¹«±â/º¸Á¶¹«±â
    Melee,           //±ÙÁ¢¹«±â
    Throwable,       //¼ö·ùÅº·ù
    
    Attach           //¹«±â ºÎÂø¹°
};

enum class TAG_RIFLE
{
    Primary,
    Secondary,
    None
};

enum class TAG_ATTACH_POINT
{
    Primary_Muzzle,     //¼ÒÀ½±â, ¼Ò¿°±â µî
    Primary_LowerRail,  //¼öÁ÷¼ÕÀâÀÌ µî
    Primary_UpperRail,  //½ºÄÚÇÁ·ù
    Primary_Mag,        //ÅºÃ¢
     
    Secondary_Muzzle,
    Secondary_LowerRail,
    Secondary_UpperRail,
    Secondary_Mag,
};

enum class TAG_CHARACTER_PART
{
    UpperBody,
    LowerBody
};

