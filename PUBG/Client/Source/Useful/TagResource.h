#pragma once
/* 리소스 관련 enum들 */

//1. 정적인 리소스 enum
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
    // SR
    Kar98k,

//    
    COUNT
};

//2. 애니메이션이 있는 리소스 enum
enum class TAG_RES_ANIM
{
    Lobby_Anim
};