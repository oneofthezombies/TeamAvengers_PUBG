#pragma once
/* 리소스 관련 enum들 */

//정적인 리소스 -------------------------------------
//TODO: 아직 UI랑 이팩트는 안했음
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

// Height Map
    HeightMap,
//    
    COUNT
};


//애니메이션이 있는 리소스 -------------------------------------
enum class TAG_RES_ANIM_WEAPON
{
    Idle,
//Weapon - 총 자체의 애니메이션
    QBZ_Anim,
    Kar98k_Anim
};

enum class TAG_RES_ANIM_CHARACTER
{
//Character
//1. 해당 무기를 들고있을 때 캐릭터의 애니메이션
    Weapon_QBZ_Character,
    Weapon_QBZ_Character_FPP,

    Weapon_Kar98k_Character,
    Weapon_Kar98k_Character_FPP,

//2. 무기를 들고있지 않을 때 애니메이션
    Unarmed_Combined,
    Unarmed_Locomotion,
    Unarmed_Jump, //스킨드매쉬 로드용

    Unarmed_Locomotion_FPP,
    Unarmed_Jump_FPP,
    Unarmed_Pickup_FPP,
    Unarmed_Attack_FPP,

//3. 무기를 들고있을 때 애니메이션
    Rifle_Locomotion,
    Rifle_Combined,

    Rifle_Locomotion_FPP,
    Rifle_Pickup_FPP,

//4. 힐템을 사용할 때 애니메이션
    Healing,
    Healing_FPP,

//5. 기절과 관련한 애니메이션
    DBNO,

//6. for test
    ForTest,

    COUNT
};

enum class TAG_RES_ANIM_EQUIPMENT
{
    Armor_Lv1_Anim,
    Back_Lv1_Anim,
    Head_Lv1_Anim
};
