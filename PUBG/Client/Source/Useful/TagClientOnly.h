#pragma once

// TagCommon.h 
// including tag list
//
// TAG_REQUEST

// TagClientOnly.h
// including tag list
// 
// TAG_OBJECT
// TAG_COLLISION
// TAG_COLLIDER_CHARACTER_PART
// TAG_ANIM_CHARACTER
// TAG_RENDERER
// TAG_SCENE
// TAG_FONT
// TAG_SOUND
// TAG_CAMERA

enum class TAG_OBJECT
{
    IDLE,
};

enum class TAG_COLLISION
{
    IDLE,
    OBSTACLE,
    BODY_OF_PLAYER_1,
    BODY_OF_PLAYER_2,
    BODY_OF_PLAYER_3,
    BODY_OF_PLAYER_4,
    DAMAGE_OF_PLAYER_1,
    DAMAGE_OF_PLAYER_2,
    DAMAGE_OF_PLAYER_3,
    DAMAGE_OF_PLAYER_4,
};

/*

author : Team_Avengers
title : collider character part chart

https://docs.google.com/spreadsheets/d/1khCjEWCtUUwS7lJogOEOQPfJHVnhsOMDu277mzoVTQ4/edit#gid=951342423

*/

enum class TAG_COLLIDER_CHARACTER_PART
{
    HEAD,            // ¸Ó¸®
    NECK,            // ¸ñ
    BREAST,          // °¡½¿
    STOMACH_UPPER,   // ¹è_»ó
    STOMACH_LOWER,   // ¹è_ÇÏ
    WAIST,           // Çã¸®
    CLAVICLE_LEFT,   // ¼â°ñ_ÁÂ
    CLAVICLE_RIGHT,  // ¼â°ñ_¿ì
    ARM_LEFT_UPPER,  // ÆÈ_ÁÂ»ó
    ARM_LEFT_LOWER,  // ÆÈ_ÁÂÇÏ
    ARM_RIGHT_UPPER, // ÆÈ_¿ì»ó
    ARM_RIGHT_LOWER, // ÆÈ_¿ìÇÏ
    HAND_LEFT,       // ¼Õ_ÁÂ
    HAND_RIGHT,      // ¼Õ_¿ì
    LEG_LEFT_UPPER,  // ´Ù¸®_ÁÂ»ó
    LEG_LEFT_LOWER,  // ´Ù¸®_ÁÂÇÏ
    LEG_RIGHT_UPPER, // ´Ù¸®_¿ì»ó
    LEG_RIGHT_LOWER, // ´Ù¸®_¿ìÇÏ
    FOOT_LEFT,       // ¹ß_ÁÂ
    FOOT_RIGHT,      // ¹ß_¿ì
};

enum class TAG_ANIM_CHARACTER
{
    Melee_Combat_Stand_Idle_Still = 11,
    Melee_Combat_Stand_Walk_F = 10,
};

enum class TAG_RENDERER
{
    DEFAULT,
    EFFECT_MESH,
    SKINNED_MESH,
    COLLIDER,
    BOX_COLLIDER,
    SPHERE_COLLIDER,
};

enum class TAG_SCENE
{
    LOADING,
    LOGIN,
    PLAY,
};

enum class TAG_FONT
{
    DEFAULT,
};

enum class TAG_SOUND
{
    IDLE,
};

enum class TAG_CAMERA
{
    FREE,
};

enum class TAG_RESOURCE
{
    Character_Female_Animation_DBNO,
    Character_Female_Animation_Healing,
    Character_Female_Animation_Healing_FPP,
    Character_Female_Animation_Lobby,
    Character_Female_Animation_Rifle_DoorOpen_And_Pickup,
    Character_Female_Animation_Rifle_Idling,
    Character_Female_Animation_Rifle_Jump,
    Character_Female_Animation_Rifle_Landing,
    Character_Female_Animation_Rifle_Locomotion_Crouch,
    Character_Female_Animation_Rifle_Locomotion_FPP,
    Character_Female_Animation_Rifle_Locomotion_Prone,
    Character_Female_Animation_Rifle_Locomotion_Stand,
    Character_Female_Animation_Rifle_OnBody,
    Character_Female_Animation_Rifle_Pickup_FPP,
    Character_Female_Animation_Rifle_Prone_PrimarySlot_OnHand,
    Character_Female_Animation_Rifle_Prone_SecondarySlot_OnHand,
    Character_Female_Animation_Rifle_Stand_PrimarySlot_OnHand,
    Character_Female_Animation_Rifle_Stand_SecondarySlot_OnHand,
    Character_Female_Animation_Rifle_Transition,
    Character_Female_Animation_Unarmed_Attack,
    Character_Female_Animation_Unarmed_Attack_FPP,
    Character_Female_Animation_Unarmed_DoorOpen_And_Pickup,
    Character_Female_Animation_Unarmed_Idling,
    Character_Female_Animation_Unarmed_Jump,
    Character_Female_Animation_Unarmed_Jump_FPP,
    Character_Female_Animation_Unarmed_Landing,
    Character_Female_Animation_Unarmed_Locomotion_Crouch,
    Character_Female_Animation_Unarmed_Locomotion_FPP,
    Character_Female_Animation_Unarmed_Locomotion_Prone,
    Character_Female_Animation_Unarmed_Locomotion_Stand,
    Character_Female_Animation_Unarmed_Pickup_FPP,
    Character_Female_Animation_Unarmed_Transition,
    Character_Female_Animation_Weapon_Kar98k_Character,
    Character_Female_Animation_Weapon_Kar98k_Character_FPP,
    Character_Female_Animation_Weapon_QBZ_Character,
    Character_Female_Animation_Weapon_QBZ_Character_FPP,
    Character_Female_Model_Character_F
};
