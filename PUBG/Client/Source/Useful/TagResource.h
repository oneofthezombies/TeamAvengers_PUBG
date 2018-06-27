#pragma once
/* ���ҽ� ���� enum�� */

//////////////////////////////������ ���ҽ�
//TODO: ���� UI�� ����Ʈ�� ������
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

//////////////////////////////�ִϸ��̼��� �ִ� ���ҽ�
//���̶� ĳ���Ͱ� �ִϸ��̼��� ����
enum class TAG_RES_ANIM
{
//Weapon - �� ��ü�� �ִϸ��̼�
    //OBZ
    QBZ_Anim,
    //Kar98k
    Kar98k_Anim,

//Character
//0. �κ�� �ִϸ��̼�
    Lobby,
//1. �ش� ���⸦ ������� �� ĳ������ �ִϸ��̼�
    Weapon_QBZ_Character,
    Weapon_QBZ_Character_FPP,

    Weapon_Kar98k_Character,
    Weapon_Kar98k_Character_FPP,

//2. ���⸦ ������� ���� �� �ִϸ��̼�
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

//3. ���⸦ ������� �� �ִϸ��̼�
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

//4. ������ ����� �� �ִϸ��̼�
    Healing,
    Healing_FPP,

//5. ������ ������ �ִϸ��̼�
    DBNO,

    COUNT
};

//////////////////////////////�ִϸ��̼� ����Ʈ
enum class TAG_ANI_SET_WEAPON
{
    //�� - �� ��ü�� �ִϸ��̼�
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
//ĳ����
//0. �κ�� �ִϸ��̼�
    //Lobby(4)
    Lobby_P1_NotReady,
    Lobby_P1_NotReady_Ready,
    Lobby_P1_Ready,
    Lobby_P1_Ready_NotReady,

//1. �ش繫�⸦ ������� �� ĳ������ �ִϸ��̼�
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

//2. ���⸦ ������� ���� �� �ִϸ��̼�
    //Unarmed_Transition(6) - �ڼ� ���� ���� ex. �ޱ׷��ִٰ� -> ���帰��
    Unarmed_Combat_Crouch_Prone, //�ޱ׸��� -> ���帮��
    Unarmed_Combat_Crouch_Stand, //�ޱ׸��� -> ����
    Unarmed_Combat_Prone_Crouch, //���帮�� -> �ޱ׸���
    Unarmed_Combat_Prone_Stand,  //���帮�� -> ����
    Unarmed_Combat_Stand_Crouch, //���� -> �ޱ׸���
    Unarmed_Combat_Stand_Prone,  //���� -> ���帮��

    //Unarmed_Locomotion_Stand(21)
        //- ���� �޸���(8)
    Unarmed_Combat_Stand_Run_B,
    Unarmed_Combat_Stand_Run_BL,
    Unarmed_Combat_Stand_Run_BR,
    Unarmed_Combat_Stand_Run_F,
    Unarmed_Combat_Stand_Run_FL,
    Unarmed_Combat_Stand_Run_FR,
    Unarmed_Combat_Stand_Run_L,
    Unarmed_Combat_Stand_Run_R,
        //- ���� �����Ѵ�(5)
    Unarmed_Combat_Stand_Sprint_F,
    Unarmed_Combat_Stand_Sprint_FL,
    Unarmed_Combat_Stand_Sprint_FR,
    Unarmed_Combat_Stand_Sprint_L,
    Unarmed_Combat_Stand_Sprint_R,
        //- ���� �ȴ´�(8)
    Unarmed_Combat_Stand_Walk_B,
    Unarmed_Combat_Stand_Walk_BL,
    Unarmed_Combat_Stand_Walk_BR,
    Unarmed_Combat_Stand_Walk_F,
    Unarmed_Combat_Stand_Walk_FL,
    Unarmed_Combat_Stand_Walk_FR,
    Unarmed_Combat_Stand_Walk_L,
    Unarmed_Combat_Stand_Walk_R,

    //Unarmed_Locomotion_Prone
        //- ������� �޸���(8)
    Unarmed_Combat_Prone_Run_B,
    Unarmed_Combat_Prone_Run_BL,
    Unarmed_Combat_Prone_Run_BR,
    Unarmed_Combat_Prone_Run_F,
    Unarmed_Combat_Prone_Run_FL,
    Unarmed_Combat_Prone_Run_FR,
    Unarmed_Combat_Prone_Run_L,
    Unarmed_Combat_Prone_Run_R,
        //- ������� �ȴ´�(8)
    Unarmed_Combat_Prone_Walk_B,
    Unarmed_Combat_Prone_Walk_BL,
    Unarmed_Combat_Prone_Walk_BR,
    Unarmed_Combat_Prone_Walk_F,
    Unarmed_Combat_Prone_Walk_FL,
    Unarmed_Combat_Prone_Walk_FR,
    Unarmed_Combat_Prone_Walk_L,
    Unarmed_Combat_Prone_Walk_R,

    //Unarmed_Locomotion_Crouch(21)
        //- �ޱ׷��� �޸���(8)
    Unarmed_Combat_Crouch_Run_B,
    Unarmed_Combat_Crouch_Run_BL,
    Unarmed_Combat_Crouch_Run_BR,
    Unarmed_Combat_Crouch_Run_F,
    Unarmed_Combat_Crouch_Run_FL,
    Unarmed_Combat_Crouch_Run_FR,
    Unarmed_Combat_Crouch_Run_L,
    Unarmed_Combat_Crouch_Run_R,
        //- �ޱ׷��� �����Ѵ�(5)
    Unarmed_Combat_Crouch_Sprint_F,
    Unarmed_Combat_Crouch_Sprint_FL,
    Unarmed_Combat_Crouch_Sprint_FR,
    Unarmed_Combat_Crouch_Sprint_L,
    Unarmed_Combat_Crouch_Sprint_R,
        //- �ޱ׷��� �ȴ´�(8)
    Unarmed_Combat_Crouch_Walk_B,
    Unarmed_Combat_Crouch_Walk_BL,
    Unarmed_Combat_Crouch_Walk_BR,
    Unarmed_Combat_Crouch_Walk_F,
    Unarmed_Combat_Crouch_Walk_FL,
    Unarmed_Combat_Crouch_Walk_FR,
    Unarmed_Combat_Crouch_Walk_L,
    Unarmed_Combat_Crouch_Walk_R,

    //Unarmed_Locomotion_FPP
        //- �ޱ׷��� �����Ѵ�(3)
    Unarmed_Combat_Crouch_Sprint_F_FPP,
    Unarmed_Combat_Crouch_Sprint_FL_FPP,
    Unarmed_Combat_Crouch_Sprint_FR_FPP,
        //- ������� �޸���(8)
    Unarmed_Combat_Prone_Run_B_FPP,
    Unarmed_Combat_Prone_Run_BL_FPP,
    Unarmed_Combat_Prone_Run_BR_FPP,
    Unarmed_Combat_Prone_Run_F_FPP,
    Unarmed_Combat_Prone_Run_FL_FPP,
    Unarmed_Combat_Prone_Run_FR_FPP,
    Unarmed_Combat_Prone_Run_L_FPP,
    Unarmed_Combat_Prone_Run_R_FPP,
        //- ������� �ȴ´�(8)
    Unarmed_Combat_Prone_Walk_B_FPP,
    Unarmed_Combat_Prone_Walk_BL_FPP,
    Unarmed_Combat_Prone_Walk_BR_FPP,
    Unarmed_Combat_Prone_Walk_F_FPP,
    Unarmed_Combat_Prone_Walk_FL_FPP,
    Unarmed_Combat_Prone_Walk_FR_FPP,
    Unarmed_Combat_Prone_Walk_L_FPP,
    Unarmed_Combat_Prone_Walk_R_FPP,
        //- ���� �����Ѵ�(3)
    Unarmed_Combat_Stand_Sprint_F_FPP,
    Unarmed_Combat_Stand_Sprint_FL_FPP,
    Unarmed_Combat_Stand_Sprint_FR_FPP,

    //Unarmed_Landing(3)
    Unarmed_Combat_Fall_Landing_Additive, //������ ����
    Unarmed_Combat_Fall_Landing_Extreme,  //�����ϴ� �Ѿ�������
    Unarmed_Combat_Fall_Landing_Hard,     //���ϴ� ����

    //Unarmed_Jump(2)
    Unarmed_Combat_Jump_F,                //������ ����
    Unarmed_Combat_Jump_Stationary,       //���ڸ� ����
    //Unarmed_Jump_FPP(2)
    Unarmed_Combat_Jump_F_FPP,
    Unarmed_Combat_Jump_Stationary_FPP,

    //Unarmed_Idling(3)
    Unarmed_Combat_Crouch_Idling_1,     
    Unarmed_Combat_Prone_Idling_1,
    Unarmed_Combat_Stand_Idling_1,

    //Unarmed_DoorOpen_And_Pickup(6)
        //- ������(3)
    Unarmed_Combat_Crouch_DoorOpen,
    Unarmed_Combat_Prone_DoorOpen,
    Unarmed_Combat_Stand_DoorOpen,
        //- �ٴڿ� �ִ� �����ݱ�(3)
    Unarmed_Combat_Crouch_Pickup_Low,
    Unarmed_Combat_Prone_Pickup_Low,
    Unarmed_Combat_Stand_Pickup_Low,

    //Unarmed_Pickup_FPP(3)
    Unarmed_Combat_Crouch_Pickup_FPP,
    Unarmed_Combat_Prone_Pickup_FPP,
    Unarmed_Combat_Stand_Pickup_FPP,

    //Unarmed_Attack(7)
    Unarmed_Combat_Crouch_Attack_1,        //�ޱ׸�ä �����ָ�
    Unarmed_Combat_Crouch_Attack_2,        //�ޱ׸�ä ���� �޼��ָ�
    Unarmed_Combat_Crouch_Attack_3,        //�ޱ׸�ä ������ �޼��ָ�

    Unarmed_Combat_Fullbody_PowerAttack_1, //������ġ

    Unarmed_Combat_Upperbody_Attack_1,     //���� �����ָ�
    Unarmed_Combat_Upperbody_Attack_2,     //���� ���� �޼��ָ�
    Unarmed_Combat_Upperbody_Attack_3,     //���� ������ �޼��ָ�

    //Unarmed_Attack_FPP(6)
    Unarmed_Combat_Crouch_Attack_1_FPP,
    Unarmed_Combat_Crouch_Attack_2_FPP,
    Unarmed_Combat_Crouch_Attack_3_FPP,

    Unarmed_Combat_Upperbody_Attack_1_FPP,
    Unarmed_Combat_Upperbody_Attack_2_FPP,
    Unarmed_Combat_Upperbody_Attack_3_FPP,

//3. ���⸦ ������� �� �ִϸ��̼�
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
    Rifle_Combat_Prone_PrimarySlot_OnHand,         //���帰ä �ֹ��⸦ �տ� ���
    Rifle_Combat_Prone_SecondarySlot_OnHand,       //���帰ä �������⸦ �տ� ���
    Rifle_Combat_Stand_PrimarySlot_OnHand,         //���� �ֹ��⸦ �տ� ���
    Rifle_Combat_Stand_SecondarySlot_OnHand,       //���� �������⸦ �տ� ���

    //Rifle_OnBody(4)
    Rifle_Combat_Prone_PrimarySlot,          //����� �ֹ��⸦ � �Ǵ�
    Rifle_Combat_Prone_SecondarySlot,        //����� �������⸦ � �Ǵ�
    Rifle_Combat_Stand_PrimarySlot_Static,   //���� �ֹ��⸦ � �Ǵ�
    Rifle_Combat_Stand_SecondarySlot_Static, //���� �������⸦ � �Ǵ�

    //Rifle_Locomotion_Stand
        //- ���� �޸���(8)
    Rifle_Combat_Stand_Run_B,
    Rifle_Combat_Stand_Run_BL,
    Rifle_Combat_Stand_Run_BR,
    Rifle_Combat_Stand_Run_F,
    Rifle_Combat_Stand_Run_FL,
    Rifle_Combat_Stand_Run_FR,
    Rifle_Combat_Stand_Run_L,
    Rifle_Combat_Stand_Run_R,
        //- ���� �����Ѵ�(13)
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
        //- ���� �ȴ´�(8)
    Rifle_Combat_Stand_Walk_B,
    Rifle_Combat_Stand_Walk_BL,
    Rifle_Combat_Stand_Walk_BR,
    Rifle_Combat_Stand_Walk_F,
    Rifle_Combat_Stand_Walk_FL,
    Rifle_Combat_Stand_Walk_FR,
    Rifle_Combat_Stand_Walk_L,
    Rifle_Combat_Stand_Walk_R,

    //Rifle_Locomotion_Prone(15)
        //- ������� �޸���(8)
    Rifle_Combat_Prone_Run_B,
    Rifle_Combat_Prone_Run_BL,
    Rifle_Combat_Prone_Run_BR,
    Rifle_Combat_Prone_Run_F,
    Rifle_Combat_Prone_Run_FL,
    Rifle_Combat_Prone_Run_FR,
    Rifle_Combat_Prone_Run_L,
    Rifle_Combat_Prone_Run_R,
        //- ������� �ȴ´�(7)
    Rifle_Combat_Prone_Walk_BL,
    Rifle_Combat_Prone_Walk_BR,
    Rifle_Combat_Prone_Walk_F,
    Rifle_Combat_Prone_Walk_FL,
    Rifle_Combat_Prone_Walk_FR,
    Rifle_Combat_Prone_Walk_L,
    Rifle_Combat_Prone_Walk_R,

    //Rifle_Locomotion_Crouch
        //- �ޱ׸�ä �޸���(8)
    Rifle_Combat_Crouch_Run_B,
    Rifle_Combat_Crouch_Run_BL,
    Rifle_Combat_Crouch_Run_BR,
    Rifle_Combat_Crouch_Run_F,
    Rifle_Combat_Crouch_Run_FL,
    Rifle_Combat_Crouch_Run_FR,
    Rifle_Combat_Crouch_Run_L,
    Rifle_Combat_Crouch_Run_R,
        //- �ޱ׸�ä �����Ѵ�(5)
    Rifle_Combat_Crouch_Sprint_F,
    Rifle_Combat_Crouch_Sprint_FL,
    Rifle_Combat_Crouch_Sprint_FR,
    Rifle_Combat_Crouch_Sprint_L,
    Rifle_Combat_Crouch_Sprint_R,
        //- �ޱ׸�ä �ȴ´�(8)
    Rifle_Combat_Crouch_Walk_B,
    Rifle_Combat_Crouch_Walk_BL,
    Rifle_Combat_Crouch_Walk_BR,
    Rifle_Combat_Crouch_Walk_F,
    Rifle_Combat_Crouch_Walk_FL,
    Rifle_Combat_Crouch_Walk_FR,
    Rifle_Combat_Crouch_Walk_L,
    Rifle_Combat_Crouch_Walk_R,

    //Rifle_Locomotion_FPP(9)
        //- �ޱ׸�ä �����Ѵ�(3)
    Rifle_Combat_Crouch_Sprint_F_FPP,
    Rifle_Combat_Crouch_Sprint_FL_FPP,
    Rifle_Combat_Crouch_Sprint_FR_FPP,
        //- ���� �����Ѵ�(6)
    Rifle_Combat_Stand_Sprint_F_WeaponDown_FPP,
    Rifle_Combat_Stand_Sprint_F_WeaponInView_FPP,
    Rifle_Combat_Stand_Sprint_FL_BigGun_FPP,
    Rifle_Combat_Stand_Sprint_FL_WeaponDown_FPP,
    Rifle_Combat_Stand_Sprint_FR_BigGun_FPP,
    Rifle_Combat_Stand_Sprint_FR_WeaponDonw_FPP,

    //Rifle_Landing(2)
    Rifle_Combat_Fall_Landing_Extreme,      //������ �����ϴ� �Ѿ���
    Rifle_Combat_Fall_Landing_Hard,         //������ �� ����

    //Rifle_Jump(4)
    Rifle_Combat_Jump_F,                     //������ ����
    Rifle_Combat_Jump_B,                     //�ڷ� ����
    Rifle_Combat_Jump_Stationary_Full_001,
    Rifle_Combat_Jump_F_Start_Aimed,

    //Rifle_Idling(10)
    Rifle_Combat_Crouch_Base,               //���⸦ ��ä �ޱ׷��ִ´�(��������x)
    Rifle_Combat_Crouch_Low_Idle_Still,     //���⸦ ��ä �ޱ׷��ִ´�(������)

    Rifle_Combat_Prone_Base,                //���⸦ ��ä ������ִ´�(��������x)
    Rifle_Combat_Prone_Low_Idle_Still,      //���⸦ ��ä ������ִ´�(������)

    Rifle_Combat_Stand_Aim,                 //����(��������x)
    Rifle_Combat_Stand_Base,                //�⺻(��������x)
    Rifle_Combat_Stand_Base_LocoIdle,        
    Rifle_Combat_Stand_Hipfire,             //�����ػ��(��������x, LocoIdle�� ����ġ�� ����)
    Rifle_Combat_Stand_Low_Base,            //�ѱ��� �Ʒ��� ����(��������x)
    Rifle_Combat_Stand_Low_Idle_Still_Very, //LocoIdle�� ���� ����ġ ����(�����δ�)

    //Rifle_DoorOpen_And_Pickup(6)
        //- ������(3)
    Rifle_Combat_Crouch_DoorOpen,
    Rifle_Combat_Prone_DoorOpen,
    Rifle_Combat_Stand_DoorOpen,
        //- �ٴڿ� �ִ� ���� ����(3)
    Rifle_Combat_Crouch_Pickup_Low,
    Rifle_Combat_Prone_Pickup_Low,
    Rifle_Combat_Stand_Pickup_Low,

    //Rifle_Pickup_FPP(3)
    Rifle_Combat_Crouch_Pickup_FPP,
    Rifle_Combat_Prone_Pickup_FPP,
    Rifle_Combat_Stand_Pickup_FPP,

//4. ������ ����� �� �ִϸ��̼�
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

//5. ������ ������ �ִϸ��̼�
   //DBNO(14)
    DBNO_Enter,                 //���ִٰ� �Ѹ¾��� ��
    DBNO_Enter_From_Crouch,     //�ޱ׷��ִ� �Ѹ¾��� ��
    DBNO_Enter_From_Prone,      //������ִ� �Ѹ¾��� ��
    DBNO_Exit_To_Crouch,        //�ޱ׷��ִ� ��ݻ��¿��� �������ö�
   
    DBNO_Idle,
        //�Ѹ��� ���·� ������ �� 
    DBNO_Move_B,               
    DBNO_Move_BL,
    DBNO_Move_BR,
    DBNO_Move_F,
    DBNO_Move_FL,
    DBNO_Move_FR,
    DBNO_Move_L,
    DBNO_Move_R,
        //���� �һ���ų ��
    DBNO_Revive,                 

//
    COUNT
};