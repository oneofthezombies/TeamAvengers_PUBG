#pragma once
/* ���ҽ� ���� enum�� */

//������ ���ҽ� -------------------------------------
//TODO: ���� UI�� ����Ʈ�� ������
enum class TAG_RES_STATIC
{
//Item
    DeathDropBox,

    //Ammunition
    Ammo_5_56mm,
    Ammo_7_62mm,

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


//�ִϸ��̼��� �ִ� ���ҽ� -------------------------------------
enum class TAG_RES_ANIM_WEAPON
{
//Weapon - �� ��ü�� �ִϸ��̼�
    QBZ_Anim,
    Kar98k_Anim
};

enum class TAG_RES_ANIM_CHARACTER
{
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

