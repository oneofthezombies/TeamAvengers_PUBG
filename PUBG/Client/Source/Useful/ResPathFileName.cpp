#include "stdafx.h"
#include "ResPathFileName.h"

pair<string, string> ResPathFileName::Get(const TAG_RES_STATIC tag)
{
    string filePath, fileName;
    switch (tag)
    {
        //Item ------------------------------------
        {
            case TAG_RES_STATIC::DeathDropBox:
            filePath = "Item/";
            fileName = "DeathDropBox";
            break;

            //Ammunition
            {
            case TAG_RES_STATIC::Ammu_5_56mm:
            filePath = "Item/Ammunition/";
            fileName = "Ammu_5_56mm";
            break;

            case TAG_RES_STATIC::Ammu_7_62mm:
            filePath = "Item/Ammunition/";
            fileName = "Ammu_7_62mm";
            break;
            }

            //Attachment
            {
            case TAG_RES_STATIC::ACOG:
                filePath = "Item/Attachment/";
                fileName = "ACOG";
                break;

            case TAG_RES_STATIC::ACOG_Setting:
                filePath = "Item/Attachment/";
                fileName = "ACOG_Setting";
                break;

            case TAG_RES_STATIC::Aimpoint2X:
                filePath = "Item/Attachment/";
                fileName = "Aimpoint2X";
                break;

            case TAG_RES_STATIC::Aimpoint2X_Setting:
                filePath = "Item/Attachment/";
                fileName = "Aimpoint2X_Setting";
                break;

            case TAG_RES_STATIC::RedDot:
                filePath = "Item/Attachment/";
                fileName = "RedDot";
                break;

            case TAG_RES_STATIC::RedDot_Setting:
                filePath = "Item/Attachment/";
                fileName = "RedDot_Setting";
                break;
            }

            //Consumable
            {
            case TAG_RES_STATIC::Bandage:
                filePath = "Item/Consumable/";
                fileName = "Bandage";
                break;

            case TAG_RES_STATIC::FirstAidKit:
                filePath = "Item/Consumable/";
                fileName = "FirstAidKit";
                break;

            case TAG_RES_STATIC::MedKit:
                filePath = "Item/Consumable/";
                fileName = "MedKit";
                break;
            }

            //Equipment
            {
            case TAG_RES_STATIC::Armor_Lv1:
                filePath = "Item/Equipment/";
                fileName = "Armor_Lv1";
                break;

            case TAG_RES_STATIC::Back_Lv1:
                filePath = "Item/Equipment/";
                fileName = "Back_Lv1";
                break;

            case TAG_RES_STATIC::Head_Lv1:
                filePath = "Item/Equipment/";
                fileName = "Head_Lv1";
                break;
            }
        }
        //Map ------------------------------------
        {
            //Building
            {
            case TAG_RES_STATIC::AbandonedTownHall:
                filePath = "Map/Building/";
                fileName = "AbandonedTownHall";
                break;

            case TAG_RES_STATIC::Church:
                filePath = "Map/Building/";
                fileName = "Church";
                break;

            case TAG_RES_STATIC::Museum:
                filePath = "Map/Building/";
                fileName = "Museum";
                break;

            case TAG_RES_STATIC::OldWoodenShed_1:
                filePath = "Map/Building/";
                fileName = "OldWoodenShed_1";
                break;

            case TAG_RES_STATIC::OldWoodenShed_2:
                filePath = "Map/Building/";
                fileName = "OldWoodenShed_2";
                break;

            case TAG_RES_STATIC::OldWoodenShed_3:
                filePath = "Map/Building/";
                fileName = "OldWoodenShed_3";
                break;

            case TAG_RES_STATIC::PoliceStation:
                filePath = "Map/Building/";
                fileName = "PoliceStation";
                break;

            case TAG_RES_STATIC::WareHouse_A:
                filePath = "Map/Building/";
                fileName = "WareHouse_A";
                break;

            case TAG_RES_STATIC::WareHouse_B:
                filePath = "Map/Building/";
                fileName = "WareHouse_B";
                break;
                    }

            //Exterior
            {
                //- AmmoBox
                {
                case TAG_RES_STATIC::AmmoBox_1:
                    filePath = "Map/Exterior/AmmoBox/";
                    fileName = "AmmoBox_1";
                    break;

                case TAG_RES_STATIC::AmmoBox_2:
                    filePath = "Map/Exterior/AmmoBox/";
                    fileName = "AmmoBox_2";
                    break;
                }

                //- BrokenVehicle
                {
                case TAG_RES_STATIC::BrokenBus:
                    filePath = "Map/Exterior/BrokenVehicle/";
                    fileName = "BrokenBus";
                    break;

                case TAG_RES_STATIC::BrokenCar:
                    filePath = "Map/Exterior/BrokenVehicle/";
                    fileName = "BrokenCar";
                    break;

                case TAG_RES_STATIC::BrokenMeshTruck:
                    filePath = "Map/Exterior/BrokenVehicle/";
                    fileName = "BrokenMeshTruck";
                    break;

                case TAG_RES_STATIC::BrokenPoliceCar:
                    filePath = "Map/Exterior/BrokenVehicle/";
                    fileName = "BrokenPoliceCar";
                    break;

                case TAG_RES_STATIC::BrokenTractorGunnyBag:
                    filePath = "Map/Exterior/BrokenVehicle/";
                    fileName = "BrokenTractorGunnyBag";
                    break;

                case TAG_RES_STATIC::BrokenUaz3151:
                    filePath = "Map/Exterior/BrokenVehicle/";
                    fileName = "BrokenUaz3151";
                    break;
                }

                //- Container
                {
                case TAG_RES_STATIC::ContainerBox_A:
                    filePath = "Map/Exterior/Container/";
                    fileName = "ContainerBox_A";
                    break;

                case TAG_RES_STATIC::ContainerBox_B:
                    filePath = "Map/Exterior/Container/";
                    fileName = "ContainerBox_B";
                    break;

                case TAG_RES_STATIC::ContainerSmall_1:
                    filePath = "Map/Exterior/Container/";
                    fileName = "ContainerSmall_1";
                    break;

                case TAG_RES_STATIC::ContainerSmall_2:
                    filePath = "Map/Exterior/Container/";
                    fileName = "ContainerSmall_2";
                    break;
                }

                //- HayBale
                {
                case TAG_RES_STATIC::HeyBale_1:
                    filePath = "Map/Exterior/HayBale/";
                    fileName = "HeyBale_1";
                    break;

                case TAG_RES_STATIC::HeyBale_2:
                    filePath = "Map/Exterior/HayBale/";
                    fileName = "HeyBale_2";
                    break;
                }

                //- MetalBarrel
                {
                case TAG_RES_STATIC::MetalBarrel_Blue:
                    filePath = "Map/Exterior/MetalBarrel/";
                    fileName = "MetalBarrel_Blue";
                    break;

                case TAG_RES_STATIC::MetalBarrel_Gray:
                    filePath = "Map/Exterior/MetalBarrel/";
                    fileName = "MetalBarrel_Gray";
                    break;

                case TAG_RES_STATIC::MetalBarrel_Green:
                    filePath = "Map/Exterior/MetalBarrel/";
                    fileName = "MetalBarrel_Green";
                    break;

                case TAG_RES_STATIC::MetalBarrel_Red:
                    filePath = "Map/Exterior/MetalBarrel/";
                    fileName = "MetalBarrel_Red";
                    break;
                }

                //- MetalFence
                {
                case TAG_RES_STATIC::MetalFence_A:
                    filePath = "Map/Exterior/MetalFence/";
                    fileName = "MetalFence_A";
                    break;

                case TAG_RES_STATIC::MetalFence_Long:
                    filePath = "Map/Exterior/MetalFence/";
                    fileName = "MetalFence_Long";
                    break;
                }

                //- PicketFence
                {
                case TAG_RES_STATIC::PicketFence_Short_A:
                    filePath = "Map/Exterior/PicketFence/";
                    fileName = "PicketFence_Short_A";
                    break;

                case TAG_RES_STATIC::PicketFence_Short_B:
                    filePath = "Map/Exterior/PicketFence/";
                    fileName = "PicketFence_Short_B";
                    break;

                case TAG_RES_STATIC::PicketFence_Short_C:
                    filePath = "Map/Exterior/PicketFence/";
                    fileName = "PicketFence_Short_C";
                    break;
                }

                //- Powerline
                {
                case TAG_RES_STATIC::Powerline_1:
                    filePath = "Map/Exterior/Powerline/";
                    fileName = "Powerline_1";
                    break;

                case TAG_RES_STATIC::Powerline_2:
                    filePath = "Map/Exterior/Powerline/";
                    fileName = "Powerline_2";
                    break;
                }

                //- RadioTower
                {
                case TAG_RES_STATIC::RadioTower_1:
                    filePath = "Map/Exterior/RadioTower/";
                    fileName = "RadioTower_1";
                    break;

                case TAG_RES_STATIC::RadioTower_2:
                    filePath = "Map/Exterior/RadioTower/";
                    fileName = "RadioTower_2";
                    break;
                }

                //- SandBag
                {
                case TAG_RES_STATIC::Sandbag_1:
                    filePath = "Map/Exterior/Sandbag/";
                    fileName = "Sandbag_1";
                    break;

                case TAG_RES_STATIC::Sandbag_2:
                    filePath = "Map/Exterior/Sandbag/";
                    fileName = "Sandbag_2";
                    break;
                }

                //- Silo
                {
                case TAG_RES_STATIC::Silo_A:
                    filePath = "Map/Exterior/Silo/";
                    fileName = "Silo_A";
                    break;

                case TAG_RES_STATIC::Silo_B:
                    filePath = "Map/Exterior/Silo/";
                    fileName = "Silo_B";
                    break;
                }

                //- Etc
                {
                case TAG_RES_STATIC::CityStreetSign:
                    filePath = "Map/Exterior/Etc/";
                    fileName = "CityStreetSign";
                    break;

                case TAG_RES_STATIC::Lighthouse:
                    filePath = "Map/Exterior/Etc/";
                    fileName = "Lighthouse";
                    break;

                case TAG_RES_STATIC::MetalShelf:
                    filePath = "Map/Exterior/Etc/";
                    fileName = "MetalShelf";
                    break;

                case TAG_RES_STATIC::TableSet:
                    filePath = "Map/Exterior/Etc/";
                    fileName = "TableSet";
                    break;

                case TAG_RES_STATIC::Tower:
                    filePath = "Map/Exterior/Etc/";
                    fileName = "Tower";
                    break;
                }
            }

            //SkySphere
            {
            case TAG_RES_STATIC::SkySphere:
                filePath = "Map/";
                fileName = "SkySphere";
                break;
            }

            //Vegetation
            {
                //- Bush
                {
                case TAG_RES_STATIC::DeadGrass:
                    filePath = "Map/Vegetation/Bush/";
                    fileName = "DeadGrass";
                    break;

                case TAG_RES_STATIC::Dogwood:
                    filePath = "Map/Vegetation/Bush/";
                    fileName = "Dogwood";
                    break;
                }

                //- Grass
                {
                case TAG_RES_STATIC::Grass_1:
                    filePath = "Map/Vegetation/Grass/";
                    fileName = "Grass_1";
                    break;

                case TAG_RES_STATIC::Grass_2:
                    filePath = "Map/Vegetation/Grass/";
                    fileName = "Grass_2";
                    break;
                }

                //- Rock
                {
                case TAG_RES_STATIC::Desert_Cover_Rock_Combine_1:
                    filePath = "Map/Vegetation/Rock/";
                    fileName = "Desert_Cover_Rock_Combine_1";
                    break;

                case TAG_RES_STATIC::Desert_Cover_Rock_Combine_2:
                    filePath = "Map/Vegetation/Rock/";
                    fileName = "Desert_Cover_Rock_Combine_2";
                    break;

                case TAG_RES_STATIC::Desert_Mashup:
                    filePath = "Map/Vegetation/Rock/";
                    fileName = "Desert_Mashup";
                    break;

                case TAG_RES_STATIC::Rock_1:
                    filePath = "Map/Vegetation/Rock/";
                    fileName = "Rock_1";
                    break;

                case TAG_RES_STATIC::Rock_2:
                    filePath = "Map/Vegetation/Rock/";
                    fileName = "Rock_2";
                    break;
                }

                //- Tree
                {
                case TAG_RES_STATIC::AlaskaCedar:
                    filePath = "Map/Vegetation/Tree/";
                    fileName = "AlaskaCedar";
                    break;

                case TAG_RES_STATIC::AmericanElem:
                    filePath = "Map/Vegetation/Tree/";
                    fileName = "AmericanElem";
                    break;

                case TAG_RES_STATIC::LondonPlane:
                    filePath = "Map/Vegetation/Tree/";
                    fileName = "LondonPlane";
                    break;
                }
            }
        }
        //Weapon ------------------------------------
        {
            //-AR
            {
            case TAG_RES_STATIC::QBZ:
                filePath = "Weapon/AR/";
                fileName = "QBZ";
                break;

            case TAG_RES_STATIC::QBZ_Mag:
                filePath = "Weapon/AR/";
                fileName = "QBZ_Mag";
                break;
            }

            //-SR
            {
            case TAG_RES_STATIC::Kar98k:
                filePath = "Weapon/SR/";
                fileName = "Kar98k";
                break;
            }
        }

        default:
        {
            assert(false &&
                "ResPathFilename::Get(TAG_RES_STATIC), default case.");
        }
        break;
    }//switch

    return make_pair("./Resource/Static/" + filePath + fileName + "/", fileName + ".X");
}

pair<string, string> ResPathFileName::Get(const TAG_RES_ANIM_WEAPON tag)
{
    string filePath, fileName;
    switch(tag)
    {
        //Weapon ------------------------------------
        {
        //AR
        case TAG_RES_ANIM_WEAPON::QBZ_Anim:
        filePath = "AR/QBZ_Anim/";
        fileName = "QBZ_Anim";
        break;

        //SR
        case TAG_RES_ANIM_WEAPON::Kar98k_Anim:
        filePath = "SR/Kar98k_Anim/";
        fileName = "Kar98k_Anim";
        break;
        }

        default:
        {
            assert(false &&
                "ResPathFilename::Get(TAG_RES_ANIM_WEAPON), default case.");
        }
        break;
    }

    return make_pair("./Resource/Anim/Weapon/" + filePath, fileName + ".X");
}
pair<string, string> ResPathFileName::Get(const TAG_RES_ANIM_CHARACTER tag)
{
    string fileName;
    switch (tag)
    {
        //Character ------------------------------------
        {
            //0. 로비
            {
            case TAG_RES_ANIM_CHARACTER::Lobby:
                fileName = "Lobby";
                break;
            }

            //1. 해당 무기를 들고있을 때 캐릭터의 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Weapon_QBZ_Character:
                fileName = "Weapon_QBZ_Character";
                break;

            case TAG_RES_ANIM_CHARACTER::Weapon_QBZ_Character_FPP:
                fileName = "Weapon_QBZ_Character_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Character:
                fileName = "Weapon_Kar98k_Character";
                break;

            case TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Character_FPP:
                fileName = "Weapon_Kar98k_Character_FPP";
                break;
            }

            //2. 무기를 들고있지 않을 때 캐릭터 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Unarmed_Transition:
                fileName = "Unarmed_Transition";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_Stand:
                fileName = "Unarmed_Locomotion_Stand";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_Prone:
                fileName = "Unarmed_Locomotion_Prone";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_Crouch:
                fileName = "Unarmed_Locomotion_Crouch";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_FPP:
                fileName = "Unarmed_Locomotion_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Landing:
                fileName = "Unarmed_Landing";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Jump:
                fileName = "Unarmed_Jump";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Jump_FPP:
                fileName = "Unarmed_Jump_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Idling:
                fileName = "Unarmed_Idling";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_DoorOpen_And_Pickup:
                fileName = "Unarmed_DoorOpen_And_Pickup";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Pickup_FPP:
                fileName = "Unarmed_Pickup_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Attack:
                fileName = "Unarmed_Attack";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Attack_FPP:
                fileName = "Unarmed_Attack_FPP";
                break;
            }

            //3. 무기를 들고있을 때 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Rifle_Transition:
                fileName = "Rifle_Transition";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Stand_SecondarySlot_OnHand:
                fileName = "Rifle_Stand_SecondarySlot_OnHand";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Stand_PrimarySlot_OnHand:
                fileName = "Rifle_Stand_PrimarySlot_OnHand";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Prone_SecondarySlot_OnHand:
                fileName = "Rifle_Prone_SecondarySlot_OnHand";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Prone_PrimarySlot_OnHand:
                fileName = "Rifle_Prone_PrimarySlot_OnHand";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_OnBody:
                fileName = "Rifle_OnBody";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Stand:
                fileName = "Rifle_Locomotion_Stand";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Prone:
                fileName = "Rifle_Locomotion_Prone";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Crouch:
                fileName = "Rifle_Locomotion_Crouch";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_FPP:
                fileName = "Rifle_Locomotion_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Landing:
                fileName = "Rifle_Landing";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Jump:
                fileName = "Rifle_Jump";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Idling:
                fileName = "Rifle_Idling";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_DoorOpen_And_Pickup:
                fileName = "Rifle_DoorOpen_And_Pickup";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Pickup_FPP:
                fileName = "Rifle_Pickup_FPP";
                break;
            }

            //4. 힐템을 사용할 때 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Healing:
                fileName = "Healing";
                break;

            case TAG_RES_ANIM_CHARACTER::Healing_FPP:
                fileName = "Healing_FPP";
                break;
            }

            //5. 기절과 관련한 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::DBNO:
                fileName = "DBNO";
                break;
            }

            //6. For test
            {
            case TAG_RES_ANIM_CHARACTER::ForTest:
                fileName = "ForTest";
                break;
            }
        }

        default:
        {
            assert(false &&
                "ResPathFilename::Get(TAG_RES_ANIM_CHARACTER), default case.");
        }
        break;
    }

    return make_pair("./Resource/Anim/Character/", fileName + ".X");
}