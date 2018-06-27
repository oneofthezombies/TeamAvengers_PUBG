#pragma once
/* 리소스 관련 enum들 */

//////////////////////////////정적인 리소스
//TODO: 아직 UI랑 이팩트는 안했음
enum class TAG_RES_STATIC
{
//Item
    DeathDropBox,

    //Ammunition
    Ammu_5_56mm,
    Ammu_7_62mm,

    //Attachment
    ACOG,
    ACOG_Setting,
    Aimpoint2X,
    Aimpoint2X_Setting,
    RedDot,
    RedDot_Setting,

    //Consumable
    Bandage,
    FirstAidKit,
    MedKit,

    //Equipment
    Armor_Lv1,
    Back_Lv1,
    Head_Lv1,

//Map
    //Building
    AbandonedTownHall,
    Church,
    Museum,
    OldWoodenShed_1,
    OldWoodenShed_2,
    OldWoodenShed_3,
    PoliceStation,
    WareHouse_A,
    WareHouse_B,

    //Exterior
        //-AmmoBox
    AmmoBox_1,
    AmmoBox_2,

        //- BrokenVehicle
    BrokenBus,
    BrokenCar,
    BrokenMeshTruck,
    BrokenPoliceCar,
    BrokenTractorGunnyBag,
    BrokenUaz3151,

        //-Container
    ContainerBox_A,
    ContainerBox_B,
    ContainerSmall_1,
    ContainerSmall_2,

        //- HayBale
    HeyBale_1,
    HeyBale_2,

        //- MetalBarrel
    MetalBarrel_Blue,
    MetalBarrel_Gray,
    MetalBarrel_Green,
    MetalBarrel_Red,

        //- MetalFence
    MetalFence_A,
    MetalFence_Long,

        //- PicketFence
    PicketFence_Short_A,
    PicketFence_Short_B,
    PicketFence_Short_C,

        //- Powerline
    Powerline_1,
    Powerline_2,

        //- RadioTower
    RadioTower_1,
    RadioTower_2,

        //- SandBag
    Sandbag_1,
    Sandbag_2,

        //- Silo
    Silo_A,
    Silo_B,

        //- Etc
    CityStreetSign,
    Lighthouse,
    MetalShelf,
    TableSet,
    Tower,

    //SkySphere
    SkySphere,

    //Vegetation
        //- Bush
    DeadGrass,
    Dogwood,

        //- Grass
    Grass_1,
    Grass_2,

        //- Rock
    Desert_Cover_Rock_Combine_1,
    Desert_Cover_Rock_Combine_2,
    Desert_Mashup,
    Rock_1,
    Rock_2,

        //- Tree
    AlaskaCedar,
    AmericanElem,
    LondonPlane,

//Weapon
    // AR
    QBZ,
    QBZ_Mag,
    // SR
    Kar98k,

//    
    COUNT
};

//////////////////////////////애니메이션이 있는 리소스
//총이랑 캐릭터가 애니메이션이 있음
enum class TAG_RES_ANIM
{
//Weapon - 총 자체의 애니메이션
    //OBZ
    QBZ_Anim,
    //Kar98k
    Kar98k_Anim,

//Character
//0. 로비씬 애니메이션
    Lobby,
//1. 해당 무기를 들고있을 때 캐릭터의 애니메이션
    Weapon_QBZ_Character,
    Weapon_QBZ_Character_FPP,

    Weapon_Kar98k_Character,
    Weapon_Kar98k_Character_FPP,

//2. 무기를 들고있지 않을 때 애니메이션
    Unarmed_Transition,
    Unarmed_Locomotion_Stand,
    Unarmed_Locomotion_Prone,
    Unarmed_Locomotion_Crouch,
    Unarmed_Locomotion_FPP,

    Unarmed_Landing,

    Unarmed_Jump,
    Unarmed_Jump_FPP,

    Unarmed_Idling,

    Unarmed_DoorOpen_And_Pickup,
    Unarmed_Pickup_FPP,

    Unarmed_Attack,
    Unarmed_Attack_FPP,

//3. 무기를 들고있을 때 애니메이션
    Rifle_Transition,

    Rifle_Stand_SecondarySlot_OnHand,
    Rifle_Stand_PrimarySlot_OnHand,
    Rifle_Prone_SecondarySlot_OnHand,
    Rifle_Prone_PrimarySlot_OnHand,

    Rifle_OnBody,

    Rifle_Locomotion_Stand,
    Rifle_Locomotion_Prone,
    Rifle_Locomotion_Crouch,
    Rifle_Locomotion_FPP,

    Rifle_Landing,

    Rifle_Jump,

    Rifle_Idling,

    Rifle_DoorOpen_And_Pickup,
    Rifle_Pickup_FPP,

//4. 힐템을 사용할 때 애니메이션
    Healing,
    Healing_FPP,

//5. 기절과 관련한 애니메이션
    DBNO,

    COUNT
};

//////////////////////////////애니메이션 리스트
enum class TAG_ANI_SET_WEAPON
{
    //총 - 총 자체의 애니메이션
    //OBZ(5)
    Weapon_QBZ_Fire,
    Weapon_QBZ_Firemode_Auto,
    Weapon_QBZ_Firemode_Semi,
    Weapon_QBZ_Reload_Charge_FPP,
    Weapon_QBZ_Reload_FPP,

    //Kar98k(6)
    Weapon_Kar98k_BoltAction_1,
    Weapon_Kar98k_BoltAction_2,
    Weapon_Kar98k_BoltAction_3,
    Weapon_Kar98k_Reload_StartLoopEnd,
    Weapon_Kar98k_Reload_Fast,
    Weapon_Kar98k_Reload_Fast_FPP,

    COUNT
};

enum class TAG_ANI_SET_CHARACTER
{
//캐릭터
//0. 로비씬 애니메이션
    //Lobby(4)
    Lobby_P1_NotReady,
    Lobby_P1_NotReady_Ready,
    Lobby_P1_Ready,
    Lobby_P1_Ready_NotReady,

//1. 해당무기를 들고있을 때 캐릭터의 애니메이션
    //Weapon_QBZ_Characert(6)
    Weapon_QBZ_Grip_Regular,
    Weapon_QBZ_Grip_VFG,
    Weapon_QBZ_Reload_Base,
    Weapon_QBZ_Reload_Charge_Base,
    Weapon_QBZ_Reload_Charge_Prone,
    Weapon_QBZ_Reload_Prone,
    //Weapon_QBZ_Character_FPP(4)
    Weapon_QBZ_Reload_Charge_FPP,
    Weapon_QBZ_Reload_Charge_Prone_FPP,
    Weapon_QBZ_Reload_FPP,
    Weapon_QBZ_Reload_Prone_FPP,

    //Weapon_Kar98k_Character(13)
    Weapon_Kar98k_BoltAction_1_Base,
    Weapon_Kar98k_BoltAction_1_Prone,
    Weapon_Kar98k_BoltAction_2_Base,
    Weapon_Kar98k_BoltAction_2_Prone,
    Weapon_Kar98k_BoltAction_3_Base,
    Weapon_Kar98k_BoltAction_3_Prone,
    Weapon_Kar98k_Grip_Carry,
    Weapon_Kar98k_Grip_Ready,
    Weapon_Kar98k_Grip_Short,
    Weapon_Kar98k_Reload_StartLoopEnd_Base,
    Weapon_Kar98k_Reload_StartLoopEnd_Prone,
    Weapon_Kar98k_ReloadFast_Base,
    Weapon_Kar98k_ReloadFast_Prone,
    //Weapon_Kar98k_Character_FPP(10)
    Weapon_Kar98k_BoltAction_1_FPP,
    Weapon_Kar98k_BoltAction_1_Prone_FPP,
    Weapon_Kar98k_BoltAction_2_FPP,
    Weapon_Kar98k_BoltAction_2_Prone_FPP,
    Weapon_Kar98k_BoltAction_3_FPP,
    Weapon_Kar98k_BoltAction_3_Prone_FPP,
    Weapon_Kar98k_Reload_StartLoopEnd_FPP,
    Weapon_Kar98k_Reload_StartLoopEnd_Prone_FPP,
    Weapon_Kar98k_ReloadFast_FPP,
    Weapon_Kar98k_ReloadFast_Prone_FPP,

//2. 무기를 들고있지 않을 때 애니메이션
    //Unarmed_Transition(6) - 자세 간의 전이 ex. 쭈그려있다가 -> 엎드린다
    Unarmed_Combat_Crouch_Prone, //쭈그리다 -> 엎드리다
    Unarmed_Combat_Crouch_Stand, //쭈그리다 -> 서다
    Unarmed_Combat_Prone_Crouch, //엎드리다 -> 쭈그리다
    Unarmed_Combat_Prone_Stand,  //엎드리다 -> 서다
    Unarmed_Combat_Stand_Crouch, //서다 -> 쭈그리다
    Unarmed_Combat_Stand_Prone,  //서다 -> 엎드리다

    //Unarmed_Locomotion_Stand(21)
        //- 서서 달린다(8)
    Unarmed_Combat_Stand_Run_B,
    Unarmed_Combat_Stand_Run_BL,
    Unarmed_Combat_Stand_Run_BR,
    Unarmed_Combat_Stand_Run_F,
    Unarmed_Combat_Stand_Run_FL,
    Unarmed_Combat_Stand_Run_FR,
    Unarmed_Combat_Stand_Run_L,
    Unarmed_Combat_Stand_Run_R,
        //- 서서 질주한다(5)
    Unarmed_Combat_Stand_Sprint_F,
    Unarmed_Combat_Stand_Sprint_FL,
    Unarmed_Combat_Stand_Sprint_FR,
    Unarmed_Combat_Stand_Sprint_L,
    Unarmed_Combat_Stand_Sprint_R,
        //- 서서 걷는다(8)
    Unarmed_Combat_Stand_Walk_B,
    Unarmed_Combat_Stand_Walk_BL,
    Unarmed_Combat_Stand_Walk_BR,
    Unarmed_Combat_Stand_Walk_F,
    Unarmed_Combat_Stand_Walk_FL,
    Unarmed_Combat_Stand_Walk_FR,
    Unarmed_Combat_Stand_Walk_L,
    Unarmed_Combat_Stand_Walk_R,

    //Unarmed_Locomotion_Prone
        //- 엎드려서 달린다(8)
    Unarmed_Combat_Prone_Run_B,
    Unarmed_Combat_Prone_Run_BL,
    Unarmed_Combat_Prone_Run_BR,
    Unarmed_Combat_Prone_Run_F,
    Unarmed_Combat_Prone_Run_FL,
    Unarmed_Combat_Prone_Run_FR,
    Unarmed_Combat_Prone_Run_L,
    Unarmed_Combat_Prone_Run_R,
        //- 엎드려서 걷는다(8)
    Unarmed_Combat_Prone_Walk_B,
    Unarmed_Combat_Prone_Walk_BL,
    Unarmed_Combat_Prone_Walk_BR,
    Unarmed_Combat_Prone_Walk_F,
    Unarmed_Combat_Prone_Walk_FL,
    Unarmed_Combat_Prone_Walk_FR,
    Unarmed_Combat_Prone_Walk_L,
    Unarmed_Combat_Prone_Walk_R,

    //Unarmed_Locomotion_Crouch(21)
        //- 쭈그려서 달린다(8)
    Unarmed_Combat_Crouch_Run_B,
    Unarmed_Combat_Crouch_Run_BL,
    Unarmed_Combat_Crouch_Run_BR,
    Unarmed_Combat_Crouch_Run_F,
    Unarmed_Combat_Crouch_Run_FL,
    Unarmed_Combat_Crouch_Run_FR,
    Unarmed_Combat_Crouch_Run_L,
    Unarmed_Combat_Crouch_Run_R,
        //- 쭈그려서 질주한다(5)
    Unarmed_Combat_Crouch_Sprint_F,
    Unarmed_Combat_Crouch_Sprint_FL,
    Unarmed_Combat_Crouch_Sprint_FR,
    Unarmed_Combat_Crouch_Sprint_L,
    Unarmed_Combat_Crouch_Sprint_R,
        //- 쭈그려서 걷는다(8)
    Unarmed_Combat_Crouch_Walk_B,
    Unarmed_Combat_Crouch_Walk_BL,
    Unarmed_Combat_Crouch_Walk_BR,
    Unarmed_Combat_Crouch_Walk_F,
    Unarmed_Combat_Crouch_Walk_FL,
    Unarmed_Combat_Crouch_Walk_FR,
    Unarmed_Combat_Crouch_Walk_L,
    Unarmed_Combat_Crouch_Walk_R,

    //Unarmed_Locomotion_FPP
        //- 쭈그려서 질주한다(3)
    Unarmed_Combat_Crouch_Sprint_F_FPP,
    Unarmed_Combat_Crouch_Sprint_FL_FPP,
    Unarmed_Combat_Crouch_Sprint_FR_FPP,
        //- 엎드려서 달린다(8)
    Unarmed_Combat_Prone_Run_B_FPP,
    Unarmed_Combat_Prone_Run_BL_FPP,
    Unarmed_Combat_Prone_Run_BR_FPP,
    Unarmed_Combat_Prone_Run_F_FPP,
    Unarmed_Combat_Prone_Run_FL_FPP,
    Unarmed_Combat_Prone_Run_FR_FPP,
    Unarmed_Combat_Prone_Run_L_FPP,
    Unarmed_Combat_Prone_Run_R_FPP,
        //- 엎드려서 걷는다(8)
    Unarmed_Combat_Prone_Walk_B_FPP,
    Unarmed_Combat_Prone_Walk_BL_FPP,
    Unarmed_Combat_Prone_Walk_BR_FPP,
    Unarmed_Combat_Prone_Walk_F_FPP,
    Unarmed_Combat_Prone_Walk_FL_FPP,
    Unarmed_Combat_Prone_Walk_FR_FPP,
    Unarmed_Combat_Prone_Walk_L_FPP,
    Unarmed_Combat_Prone_Walk_R_FPP,
        //- 서서 질주한다(3)
    Unarmed_Combat_Stand_Sprint_F_FPP,
    Unarmed_Combat_Stand_Sprint_FL_FPP,
    Unarmed_Combat_Stand_Sprint_FR_FPP,

    //Unarmed_Landing(3)
    Unarmed_Combat_Fall_Landing_Additive, //가벼운 착지
    Unarmed_Combat_Fall_Landing_Extreme,  //착지하다 넘어짐ㅋㅋ
    Unarmed_Combat_Fall_Landing_Hard,     //쾅하는 착지

    //Unarmed_Jump(2)
    Unarmed_Combat_Jump_F,                //앞으로 점프
    Unarmed_Combat_Jump_Stationary,       //제자리 점프
    //Unarmed_Jump_FPP(2)
    Unarmed_Combat_Jump_F_FPP,
    Unarmed_Combat_Jump_Stationary_FPP,

    //Unarmed_Idling(3)
    Unarmed_Combat_Crouch_Idling_1,     
    Unarmed_Combat_Prone_Idling_1,
    Unarmed_Combat_Stand_Idling_1,

    //Unarmed_DoorOpen_And_Pickup(6)
        //- 문열기(3)
    Unarmed_Combat_Crouch_DoorOpen,
    Unarmed_Combat_Prone_DoorOpen,
    Unarmed_Combat_Stand_DoorOpen,
        //- 바닥에 있는 물건줍기(3)
    Unarmed_Combat_Crouch_Pickup_Low,
    Unarmed_Combat_Prone_Pickup_Low,
    Unarmed_Combat_Stand_Pickup_Low,

    //Unarmed_Pickup_FPP(3)
    Unarmed_Combat_Crouch_Pickup_FPP,
    Unarmed_Combat_Prone_Pickup_FPP,
    Unarmed_Combat_Stand_Pickup_FPP,

    //Unarmed_Attack(7)
    Unarmed_Combat_Crouch_Attack_1,        //쭈그린채 오른주먹
    Unarmed_Combat_Crouch_Attack_2,        //쭈그린채 힘찬 왼손주먹
    Unarmed_Combat_Crouch_Attack_3,        //쭈그린채 가벼운 왼손주먹

    Unarmed_Combat_Fullbody_PowerAttack_1, //점프펀치

    Unarmed_Combat_Upperbody_Attack_1,     //서서 오른주먹
    Unarmed_Combat_Upperbody_Attack_2,     //서서 힘찬 왼손주먹
    Unarmed_Combat_Upperbody_Attack_3,     //서서 가벼운 왼손주먹

    //Unarmed_Attack_FPP(6)
    Unarmed_Combat_Crouch_Attack_1_FPP,
    Unarmed_Combat_Crouch_Attack_2_FPP,
    Unarmed_Combat_Crouch_Attack_3_FPP,

    Unarmed_Combat_Upperbody_Attack_1_FPP,
    Unarmed_Combat_Upperbody_Attack_2_FPP,
    Unarmed_Combat_Upperbody_Attack_3_FPP,

//3. 무기를 들고있을 때 애니메이션
    //Rifle_Transition(10)
    Rifle_Combat_Crouch_Aim_Stand,
    Rifle_Combat_Crouch_Base_Prone,
    Rifle_Combat_Crouch_Base_Stand,

    Rifle_Combat_Prone_Base_Crouch,
    Rifle_Combat_Prone_Base_Stand,

    Rifle_Combat_Stand_Aim_Crouch,
    Rifle_Combat_Stand_Base_Crouch,
    Rifle_Combat_Stand_Base_Prone,
    Rifle_Combat_Stand_Run_F_Prone,
    Rifle_Combat_Stand_Sprint_F_Prone,

    //Rifle_OnHand(4)
    Rifle_Combat_Prone_PrimarySlot_OnHand,         //엎드린채 주무기를 손에 든다
    Rifle_Combat_Prone_SecondarySlot_OnHand,       //엎드린채 보조무기를 손에 든다
    Rifle_Combat_Stand_PrimarySlot_OnHand,         //서서 주무기를 손에 든다
    Rifle_Combat_Stand_SecondarySlot_OnHand,       //서서 보조무기를 손에 든다

    //Rifle_OnBody(4)
    Rifle_Combat_Prone_PrimarySlot,          //엎드려 주무기를 등에 맨다
    Rifle_Combat_Prone_SecondarySlot,        //엎드려 보조무기를 등에 맨다
    Rifle_Combat_Stand_PrimarySlot_Static,   //서서 주무기를 등에 맨다
    Rifle_Combat_Stand_SecondarySlot_Static, //서서 보조무기를 등에 맨다

    //Rifle_Locomotion_Stand
        //- 서서 달린다(8)
    Rifle_Combat_Stand_Run_B,
    Rifle_Combat_Stand_Run_BL,
    Rifle_Combat_Stand_Run_BR,
    Rifle_Combat_Stand_Run_F,
    Rifle_Combat_Stand_Run_FL,
    Rifle_Combat_Stand_Run_FR,
    Rifle_Combat_Stand_Run_L,
    Rifle_Combat_Stand_Run_R,
        //- 서서 질주한다(13)
    Rifle_Combat_Stand_Sprint_F_WeaponDown,
    Rifle_Combat_Stand_Sprint_F_WeaponInView,
    Rifle_Combat_Stand_Sprint_F_WeaponUp,

    Rifle_Combat_Stand_Sprint_FL_BigGun,
    Rifle_Combat_Stand_Sprint_FL_WeaponDown,
    Rifle_Combat_Stand_Sprint_FL_WeaponUp,

    Rifle_Combat_Stand_Sprint_FR_BigGun,
    Rifle_Combat_Stand_Sprint_FR_WeaponDown,
    Rifle_Combat_Stand_Sprint_FR_WeaponUp,

    Rifle_Combat_Stand_Sprint_L_WeaponDown,
    Rifle_Combat_Stand_Sprint_L_WeaponUp,

    Rifle_Combat_Stand_Sprint_R_WeaponDown,
    Rifle_Combat_Stand_Sprint_R_WeaponUp,
        //- 서서 걷는다(8)
    Rifle_Combat_Stand_Walk_B,
    Rifle_Combat_Stand_Walk_BL,
    Rifle_Combat_Stand_Walk_BR,
    Rifle_Combat_Stand_Walk_F,
    Rifle_Combat_Stand_Walk_FL,
    Rifle_Combat_Stand_Walk_FR,
    Rifle_Combat_Stand_Walk_L,
    Rifle_Combat_Stand_Walk_R,

    //Rifle_Locomotion_Prone(15)
        //- 엎드려서 달린다(8)
    Rifle_Combat_Prone_Run_B,
    Rifle_Combat_Prone_Run_BL,
    Rifle_Combat_Prone_Run_BR,
    Rifle_Combat_Prone_Run_F,
    Rifle_Combat_Prone_Run_FL,
    Rifle_Combat_Prone_Run_FR,
    Rifle_Combat_Prone_Run_L,
    Rifle_Combat_Prone_Run_R,
        //- 엎드려서 걷는다(7)
    Rifle_Combat_Prone_Walk_BL,
    Rifle_Combat_Prone_Walk_BR,
    Rifle_Combat_Prone_Walk_F,
    Rifle_Combat_Prone_Walk_FL,
    Rifle_Combat_Prone_Walk_FR,
    Rifle_Combat_Prone_Walk_L,
    Rifle_Combat_Prone_Walk_R,

    //Rifle_Locomotion_Crouch
        //- 쭈그린채 달린다(8)
    Rifle_Combat_Crouch_Run_B,
    Rifle_Combat_Crouch_Run_BL,
    Rifle_Combat_Crouch_Run_BR,
    Rifle_Combat_Crouch_Run_F,
    Rifle_Combat_Crouch_Run_FL,
    Rifle_Combat_Crouch_Run_FR,
    Rifle_Combat_Crouch_Run_L,
    Rifle_Combat_Crouch_Run_R,
        //- 쭈그린채 질주한다(5)
    Rifle_Combat_Crouch_Sprint_F,
    Rifle_Combat_Crouch_Sprint_FL,
    Rifle_Combat_Crouch_Sprint_FR,
    Rifle_Combat_Crouch_Sprint_L,
    Rifle_Combat_Crouch_Sprint_R,
        //- 쭈그린채 걷는다(8)
    Rifle_Combat_Crouch_Walk_B,
    Rifle_Combat_Crouch_Walk_BL,
    Rifle_Combat_Crouch_Walk_BR,
    Rifle_Combat_Crouch_Walk_F,
    Rifle_Combat_Crouch_Walk_FL,
    Rifle_Combat_Crouch_Walk_FR,
    Rifle_Combat_Crouch_Walk_L,
    Rifle_Combat_Crouch_Walk_R,

    //Rifle_Locomotion_FPP(9)
        //- 쭈그린채 질주한다(3)
    Rifle_Combat_Crouch_Sprint_F_FPP,
    Rifle_Combat_Crouch_Sprint_FL_FPP,
    Rifle_Combat_Crouch_Sprint_FR_FPP,
        //- 서서 질주한다(6)
    Rifle_Combat_Stand_Sprint_F_WeaponDown_FPP,
    Rifle_Combat_Stand_Sprint_F_WeaponInView_FPP,
    Rifle_Combat_Stand_Sprint_FL_BigGun_FPP,
    Rifle_Combat_Stand_Sprint_FL_WeaponDown_FPP,
    Rifle_Combat_Stand_Sprint_FR_BigGun_FPP,
    Rifle_Combat_Stand_Sprint_FR_WeaponDonw_FPP,

    //Rifle_Landing(2)
    Rifle_Combat_Fall_Landing_Extreme,      //무기들고 착지하다 넘어짐
    Rifle_Combat_Fall_Landing_Hard,         //무기들고 쾅 착지

    //Rifle_Jump(4)
    Rifle_Combat_Jump_F,                     //앞으로 점프
    Rifle_Combat_Jump_B,                     //뒤로 점프
    Rifle_Combat_Jump_Stationary_Full_001,
    Rifle_Combat_Jump_F_Start_Aimed,

    //Rifle_Idling(10)
    Rifle_Combat_Crouch_Base,               //무기를 든채 쭈그려있는다(움직이지x)
    Rifle_Combat_Crouch_Low_Idle_Still,     //무기를 든채 쭈그려있는다(움직임)

    Rifle_Combat_Prone_Base,                //무기를 든채 엎드려있는다(움직이지x)
    Rifle_Combat_Prone_Low_Idle_Still,      //무기를 든채 엎드려있는다(움직임)

    Rifle_Combat_Stand_Aim,                 //조준(움직이지x)
    Rifle_Combat_Stand_Base,                //기본(움직이지x)
    Rifle_Combat_Stand_Base_LocoIdle,        
    Rifle_Combat_Stand_Hipfire,             //비조준사격(움직이지x, LocoIdle과 손위치가 같다)
    Rifle_Combat_Stand_Low_Base,            //총구가 아래로 향함(움직이지x)
    Rifle_Combat_Stand_Low_Idle_Still_Very, //LocoIdle과 같은 손위치 상태(움직인다)

    //Rifle_DoorOpen_And_Pickup(6)
        //- 문열기(3)
    Rifle_Combat_Crouch_DoorOpen,
    Rifle_Combat_Prone_DoorOpen,
    Rifle_Combat_Stand_DoorOpen,
        //- 바닥에 있는 물건 집기(3)
    Rifle_Combat_Crouch_Pickup_Low,
    Rifle_Combat_Prone_Pickup_Low,
    Rifle_Combat_Stand_Pickup_Low,

    //Rifle_Pickup_FPP(3)
    Rifle_Combat_Crouch_Pickup_FPP,
    Rifle_Combat_Prone_Pickup_FPP,
    Rifle_Combat_Stand_Pickup_FPP,

//4. 힐템을 사용할 때 애니메이션
    //Healing(6)
    Healing_Combat_Prone_Aidkit,
    Healing_Combat_Prone_Bandage,
    Healing_Combat_Prone_Medkit,

    Healing_Combat_Stand_Aidkit,
    Healing_Combat_Stand_Bandage,
    Healing_Combat_Stand_Medkit,
    //Healing_FPP(6)
    Healing_Combat_Prone_Aidkit_FPP,
    Healing_Combat_Prone_Bandage_FPP,
    Healing_Combat_Prone_Medkit_FPP,

    Healing_Combat_Stand_Aidkit_FPP,
    Healing_Combat_Stand_Bandage_FPP,
    Healing_Combat_Stand_Medkit_FPP,

//5. 기절과 관련한 애니메이션
   //DBNO(14)
    DBNO_Enter,                 //서있다가 총맞았을 때
    DBNO_Enter_From_Crouch,     //쭈그려있다 총맞았을 때
    DBNO_Enter_From_Prone,      //엎드려있다 총맞았을 때
    DBNO_Exit_To_Crouch,        //쭈그려있다 충격상태에서 빠져나올때
   
    DBNO_Idle,
        //총맞은 상태로 움직일 때 
    DBNO_Move_B,               
    DBNO_Move_BL,
    DBNO_Move_BR,
    DBNO_Move_F,
    DBNO_Move_FL,
    DBNO_Move_FR,
    DBNO_Move_L,
    DBNO_Move_R,
        //팀원 소생시킬 때
    DBNO_Revive,                 

//
    COUNT
};