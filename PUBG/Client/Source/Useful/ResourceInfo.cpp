#include "stdafx.h"
#include "ResourceInfo.h"

pair<string, string> ResourceInfo::GetPathFileName(const TAG_RES_STATIC tag)
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
            case TAG_RES_STATIC::Ammo_5_56mm:
            filePath = "Item/Ammunition/";
            fileName = "Ammo_5_56mm";
            break;

            case TAG_RES_STATIC::Ammo_7_62mm:
            filePath = "Item/Ammunition/";
            fileName = "Ammo_7_62mm";
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
                "ResourceInfo::GetPathFileName(TAG_RES_STATIC), default case.");
        }
        break;
    }//switch

    return make_pair("./Resource/Static/" + filePath + fileName + "/", fileName + ".X");
}

pair<string, string> ResourceInfo::GetPathFileName(const TAG_RES_ANIM_WEAPON tag)
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
                "ResourceInfo::GetPathFileName(TAG_RES_ANIM_WEAPON), default case.");
        }
        break;
    }

    return make_pair("./Resource/Anim/Weapon/" + filePath, fileName + ".X");
}
pair<string, string> ResourceInfo::GetPathFileName(const TAG_RES_ANIM_CHARACTER tag)
{
    string fileName;
    switch (tag)
    {
        //FPP
            case TAG_RES_ANIM_CHARACTER::Unarmed_Combined_FPP:
                fileName = "Unarmed_Combined_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Combined_FPP:
                fileName = "Rifle_Combined_FPP";
                break;
                
            case TAG_RES_ANIM_CHARACTER::Weapon_Combined_FPP:
                fileName = "Weapon_Combined_FPP";
                break;

            case TAG_RES_ANIM_CHARACTER::Healing_FPP:
                fileName = "Healing_FPP";
                break;

        //Character ------------------------------------
        {   
            //1. 해당 무기를 들고있을 때 캐릭터의 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Weapon_QBZ_Character:
                fileName = "Weapon_QBZ_Character";
                break;

            case TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Character:
                fileName = "Weapon_Kar98k_Character";
                break;
            }

            //2. 무기를 들고있지 않을 때 캐릭터 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Unarmed_Combined:
                fileName = "Unarmed_Combined";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion:
                fileName = "Unarmed_Locomotion";
                break;

            case TAG_RES_ANIM_CHARACTER::Unarmed_Jump:
                fileName = "Unarmed_Jump";
                break;
            }

            //3. 무기를 들고있을 때 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Rifle_Locomotion:
                fileName = "Rifle_Locomotion";
                break;

            case TAG_RES_ANIM_CHARACTER::Rifle_Combined:
                fileName = "Rifle_Combined";
                break;
            }

            //4. 힐템을 사용할 때 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::Healing:
                fileName = "Healing";
                break;
            }

            //5. 기절과 관련한 애니메이션
            {
            case TAG_RES_ANIM_CHARACTER::DBNO:
                fileName = "DBNO";
                break;
            }
        }

        default:
        {
            assert(false &&
                "ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER), default case.");
        }
        break;
    }

    return make_pair("./Resource/Anim/Character/", fileName + ".X");
}

pair<string, string> ResourceInfo::GetPathFileName(const TAG_RES_ANIM_EQUIPMENT tag)
{
    string path("");
    string filename("");

    switch (tag)
    {
    case TAG_RES_ANIM_EQUIPMENT::Armor_Lv1_Anim:
        path = getPath(tag);
        filename = getFilename(tag);
        break;

    case TAG_RES_ANIM_EQUIPMENT::Back_Lv1_Anim: 
        path = getPath(tag); 
        filename = getFilename(tag); 
        break;

    case TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim:
        path = getPath(tag);
        filename = getFilename(tag);
        break;

    default:
    {
        assert(false &&
            "ResourceInfo::GetPathFileName(), default case.");
        return make_pair("", "");
    }
    }

    return make_pair(path, filename);
}

pair<string, string> ResourceInfo::GetUIPathFileName(const TAG_RES_STATIC tag)
{
    string filePath("");
    string fileName("");
    switch (tag)
    {
        //탄약, 소모품, 무기부착물
    case TAG_RES_STATIC::Ammo_5_56mm:
        filePath = "Inventory/Item/Ammunition/";
        fileName = "icon_ammo_556mm.png";
        break;
    case TAG_RES_STATIC::Ammo_7_62mm:
        filePath = "Inventory/Item/Ammunition/";
        fileName = "icon_ammo_762mm.png";
        break;

    case TAG_RES_STATIC::Bandage:
        filePath = "Inventory/Item/Consumable/";
        fileName = "icon_Heal_Bandage.png";
        break;
    case TAG_RES_STATIC::FirstAidKit:
        filePath = "Inventory/Item/Consumable/";
        fileName = "icon_Heal_FirstAid.png";
        break;
    case TAG_RES_STATIC::MedKit:
        filePath = "Inventory/Item/Consumable/";
        fileName = "icon_Heal_MedKit.png";
        break;

    case TAG_RES_STATIC::ACOG:
        filePath = "Inventory/Item/Attachment/";
        fileName = "icon_attach_Upper_ACOG.png";
        break;
    case TAG_RES_STATIC::Aimpoint2X:
        filePath = "Inventory/Item/Attachment/";
        fileName = "icon_attach_Upper_Aimpoint2X.png";
        break;
    case TAG_RES_STATIC::RedDot:
        filePath = "Inventory/Item/Attachment/";
        fileName = "icon_attach_Upper_DotSight.png";
        break;
    
    //장비
    case TAG_RES_STATIC::Armor_Lv1:
        filePath = "Inventory/Item/Equipment/";
        fileName = "icon_equipment_Armor_Lv1.png";
        break;
    case TAG_RES_STATIC::Back_Lv1:
        filePath = "Inventory/Item/Equipment/";
        fileName = "icon_equipment_Back_Lv1.png";
        break;
    case TAG_RES_STATIC::Head_Lv1:
        filePath = "Inventory/Item/Equipment/";
        fileName = "icon_equipment_Head_Lv1.png";
        break;

    default:
        assert(false && "ResourceInfo::GetUIPathFileName(), default case.");
        break;
    }
    return make_pair("./Resource/UI/" + filePath, fileName);
}

pair<string, string> ResourceInfo::GetCharacterPathFileName()
{
    return std::make_pair("Character/", "Female");
}

bool ResourceInfo::IsItem(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Head_Lv1:
    case TAG_RES_STATIC::Armor_Lv1:
    case TAG_RES_STATIC::Back_Lv1:

    case TAG_RES_STATIC::Bandage:
    case TAG_RES_STATIC::FirstAidKit:
    case TAG_RES_STATIC::MedKit:

    case TAG_RES_STATIC::Ammo_5_56mm:
    case TAG_RES_STATIC::Ammo_7_62mm:

    case TAG_RES_STATIC::QBZ:
    case TAG_RES_STATIC::Kar98k:

    case TAG_RES_STATIC::RedDot:
    case TAG_RES_STATIC::Aimpoint2X:
    case TAG_RES_STATIC::ACOG:
        return true;

    default: return false;
    }
}

TAG_RES_ANIM_WEAPON ResourceInfo::GetTagResAnimWeapon(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ:    return TAG_RES_ANIM_WEAPON::QBZ_Anim;
    case TAG_RES_STATIC::Kar98k: return TAG_RES_ANIM_WEAPON::Kar98k_Anim;

    default:                  
        assert(false && "ResourceInfo::GetTagResAnimWeapon(), default case.");
        return TAG_RES_ANIM_WEAPON::Idle;
    }
}

string ResourceInfo::getPath(const TAG_RES_ANIM_EQUIPMENT tag)
{
    string base("./Resource/Anim/Item/Equipment/");
    switch (tag) 
    {
    case TAG_RES_ANIM_EQUIPMENT::Armor_Lv1_Anim: return base + "Armor_Lv1_Anim/";
    case TAG_RES_ANIM_EQUIPMENT::Back_Lv1_Anim: return base + "Back_Lv1_Anim/";
    case TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim: return base + "Head_Lv1_Anim/";

    default: 
    {
        assert(false && "ResourceInfo::getPath(), default case.");
        return "";
    }
    }
}

string ResourceInfo::getFilename(const TAG_RES_ANIM_EQUIPMENT tag)
{
    switch (tag)
    {
    case TAG_RES_ANIM_EQUIPMENT::Armor_Lv1_Anim: return "Armor_Lv1_Anim.X";
    case TAG_RES_ANIM_EQUIPMENT::Back_Lv1_Anim: return "Back_Lv1_Anim.X";
    case TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim: return "Head_Lv1_Anim.X";
    default:
    {
        assert(false && "ResourceInfo::getFilename(), default case.");
        return "";
    }
    }
}
