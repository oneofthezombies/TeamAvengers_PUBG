#include "stdafx.h"
#include "TagAnimation.h"

std::string TagAnimation::GetString(const TAG_ANIM_WEAPON tag)
{
    switch (tag)
    {
    //�� - �� ��ü�� �ִϸ��̼�
    //QBZ(6)
    case TAG_ANIM_WEAPON::Weapon_QBZ_Fire:              return "Weapon_QBZ_Fire";
    case TAG_ANIM_WEAPON::Weapon_QBZ_Firemode_Auto:     return "Weapon_QBZ_Firemode_Auto";
    case TAG_ANIM_WEAPON::Weapon_QBZ_Firemode_Semi:     return "Weapon_QBZ_Firemode_Semi";
    case TAG_ANIM_WEAPON::Weapon_QBZ_Reload_Charge_FPP: return "Weapon_QBZ_Reload_Charge_FPP";
    case TAG_ANIM_WEAPON::Weapon_QBZ_Reload_FPP:        return "Weapon_QBZ_Reload_FPP";
    case TAG_ANIM_WEAPON::Weapon_QBZ_Idle:              return "Weapon_QBZ_Idle";

    //Kar98k(7)
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Start:        return "Weapon_Kar98k_Reload_Start";
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Loop:         return "Weapon_Kar98k_Reload_Loop";
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_End:          return "Weapon_Kar98k_Reload_End";
    case TAG_ANIM_WEAPON::Weapon_Kar98k_BoltAction_1:        return "Weapon_Kar98k_BoltAction_1";
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Fast:         return "Weapon_Kar98k_Reload_Fast";
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Fast_FPP:     return "Weapon_Kar98k_Reload_Fast_FPP";
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Idle:                return "Weapon_Kar98k_Idle";

    case TAG_ANIM_WEAPON::COUNT: return "COUNT";
    default:
        {
            assert(false && "TagAnimToString::Get(), default case.");
            return "";
        }
    }
}

std::string TagAnimation::GetString(const TAG_ANIM_CHARACTER tag)
{
    switch (tag)
    {
    //ĳ����
    //0. �κ�� �ִϸ��̼�
    //Lobby(4)
    case TAG_ANIM_CHARACTER::Lobby_P1_NotReady:       return "Lobby_P1_NotReady";
    case TAG_ANIM_CHARACTER::Lobby_P1_NotReady_Ready: return "Lobby_P1_NotReady_Ready";
    case TAG_ANIM_CHARACTER::Lobby_P1_Ready:          return "Lobby_P1_Ready";
    case TAG_ANIM_CHARACTER::Lobby_P1_Ready_NotReady: return "Lobby_P1_Ready_NotReady";
            
    //1. �ش繫�⸦ ������� �� ĳ������ �ִϸ��̼�
    //Weapon_QBZ_Characert(6)
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Grip_Regular:        return "Weapon_QBZ_Grip_Regular";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Grip_VFG:            return "Weapon_QBZ_Grip_VFG";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Base:         return "Weapon_QBZ_Reload_Base";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_Base:  return "Weapon_QBZ_Reload_Charge_Base";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_Prone: return "Weapon_QBZ_Reload_Charge_Prone";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Prone:        return "Weapon_QBZ_Reload_Prone";

    //Weapon_QBZ_Character_FPP(4)
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_FPP:       return "Weapon_QBZ_Reload_Charge_FPP";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_Prone_FPP: return "Weapon_QBZ_Reload_Charge_Prone_FPP";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_FPP:              return "Weapon_QBZ_Reload_FPP";
    case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Prone_FPP:        return "Weapon_QBZ_Reload_Prone_FPP";
            
    //Weapon_Kar98k_Character(17)
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Base:         return "Weapon_Kar98k_Reload_Start_Base";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Base:          return "Weapon_Kar98k_Reload_Loop_Base";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Base:           return "Weapon_Kar98k_Reload_End_Base";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Prone:        return "Weapon_Kar98k_Reload_Start_Prone";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Prone:         return "Weapon_Kar98k_Reload_Loop_Prone";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Prone:          return "Weapon_Kar98k_Reload_End_Prone";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Base:         return "Weapon_Kar98k_BoltAction_1_Base";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Prone:        return "Weapon_Kar98k_BoltAction_1_Prone";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_2_Base:         return "Weapon_Kar98k_BoltAction_2_Base";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_2_Prone:        return "Weapon_Kar98k_BoltAction_2_Prone";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_3_Base:         return "Weapon_Kar98k_BoltAction_3_Base";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_3_Prone:        return "Weapon_Kar98k_BoltAction_3_Prone";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Grip_Carry:                return "Weapon_Kar98k_Grip_Carry";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Grip_Ready:                return "Weapon_Kar98k_Grip_Ready";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Grip_Short:                return "Weapon_Kar98k_Grip_Short";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Base:           return "Weapon_Kar98k_ReloadFast_Base";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Prone:          return "Weapon_Kar98k_ReloadFast_Prone";
    
    //Weapon_Kar98k_Character_FPP(14)
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_FPP:              return "Weapon_Kar98k_BoltAction_1_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Prone_FPP:        return "Weapon_Kar98k_BoltAction_1_Prone_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_2_FPP:              return "Weapon_Kar98k_BoltAction_2_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_2_Prone_FPP:        return "Weapon_Kar98k_BoltAction_2_Prone_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_3_FPP:              return "Weapon_Kar98k_BoltAction_3_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_3_Prone_FPP:        return "Weapon_Kar98k_BoltAction_3_Prone_FPP";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_FPP:              return "Weapon_Kar98k_Reload_Start_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_FPP:               return "Weapon_Kar98k_Reload_Loop_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_FPP:                return "Weapon_Kar98k_Reload_End_FPP";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Prone_FPP:        return "Weapon_Kar98k_Reload_Start_Prone_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Prone_FPP:         return "Weapon_Kar98k_Reload_Loop_Prone_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Prone_FPP:          return "Weapon_Kar98k_Reload_End_Prone_FPP";

    case TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_FPP:                return "Weapon_Kar98k_ReloadFast_FPP";
    case TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Prone_FPP:          return "Weapon_Kar98k_ReloadFast_Prone_FPP";
            
    //2. ���⸦ ������� ���� �� �ִϸ��̼�
    //Unarmed_Transition(6) - �ڼ� ���� ���� ex. �ޱ׷��ִٰ� -> ���帰��
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Prone: return "Unarmed_Combat_Crouch_Prone"; //�ޱ׸��� -> ���帮��
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Stand: return "Unarmed_Combat_Crouch_Stand"; //�ޱ׸��� -> ����
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Crouch: return "Unarmed_Combat_Prone_Crouch"; //���帮�� -> �ޱ׸���
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Stand:  return "Unarmed_Combat_Prone_Stand";  //���帮�� -> ����
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Crouch: return "Unarmed_Combat_Stand_Crouch"; //���� -> �ޱ׸���
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Prone:  return "Unarmed_Combat_Stand_Prone";  //���� -> ���帮��
            
    //Unarmed_Locomotion_Stand(21)
    //- ���� �޸���(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B:  return "Unarmed_Combat_Stand_Run_B";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BL: return "Unarmed_Combat_Stand_Run_BL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BR: return "Unarmed_Combat_Stand_Run_BR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F:  return "Unarmed_Combat_Stand_Run_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FL: return "Unarmed_Combat_Stand_Run_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FR: return "Unarmed_Combat_Stand_Run_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_L:  return "Unarmed_Combat_Stand_Run_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_R:  return "Unarmed_Combat_Stand_Run_R";
    //- ���� �����Ѵ�(5)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F:  return "Unarmed_Combat_Stand_Sprint_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL: return "Unarmed_Combat_Stand_Sprint_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR: return "Unarmed_Combat_Stand_Sprint_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_L:  return "Unarmed_Combat_Stand_Sprint_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_R:  return "Unarmed_Combat_Stand_Sprint_R";
    //- ���� �ȴ´�(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B:  return "Unarmed_Combat_Stand_Walk_B";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_BL: return "Unarmed_Combat_Stand_Walk_BL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_BR: return "Unarmed_Combat_Stand_Walk_BR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F:  return "Unarmed_Combat_Stand_Walk_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_FL: return "Unarmed_Combat_Stand_Walk_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_FR: return "Unarmed_Combat_Stand_Walk_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_L:  return "Unarmed_Combat_Stand_Walk_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_R:  return "Unarmed_Combat_Stand_Walk_R";
            
    //Unarmed_Locomotion_Prone
    //- ������� �޸���(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_B:  return "Unarmed_Combat_Prone_Run_B";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BL: return "Unarmed_Combat_Prone_Run_BL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BR: return "Unarmed_Combat_Prone_Run_BR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_F:  return "Unarmed_Combat_Prone_Run_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FL: return "Unarmed_Combat_Prone_Run_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FR: return "Unarmed_Combat_Prone_Run_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_L:  return "Unarmed_Combat_Prone_Run_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_R:  return "Unarmed_Combat_Prone_Run_R";
    //- ������� �ȴ´�(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_B:  return "Unarmed_Combat_Prone_Walk_B";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BL: return "Unarmed_Combat_Prone_Walk_BL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BR: return "Unarmed_Combat_Prone_Walk_BR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_F:  return "Unarmed_Combat_Prone_Walk_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FL: return "Unarmed_Combat_Prone_Walk_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FR: return "Unarmed_Combat_Prone_Walk_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_L:  return "Unarmed_Combat_Prone_Walk_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_R:  return "Unarmed_Combat_Prone_Walk_R";
            
    //Unarmed_Locomotion_Crouch(21)
    //- �ޱ׷��� �޸���(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_B:  return "Unarmed_Combat_Crouch_Run_B";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BL: return "Unarmed_Combat_Crouch_Run_BL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BR: return "Unarmed_Combat_Crouch_Run_BR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_F:  return "Unarmed_Combat_Crouch_Run_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FL: return "Unarmed_Combat_Crouch_Run_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FR: return "Unarmed_Combat_Crouch_Run_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_L:  return "Unarmed_Combat_Crouch_Run_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_R:  return "Unarmed_Combat_Crouch_Run_R";
    //- �ޱ׷��� �����Ѵ�(5)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_F:  return "Unarmed_Combat_Crouch_Sprint_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FL: return "Unarmed_Combat_Crouch_Sprint_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FR: return "Unarmed_Combat_Crouch_Sprint_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_L:  return "Unarmed_Combat_Crouch_Sprint_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_R:  return "Unarmed_Combat_Crouch_Sprint_R";
    //- �ޱ׷��� �ȴ´�(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_B:  return "Unarmed_Combat_Crouch_Walk_B";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_BL: return "Unarmed_Combat_Crouch_Walk_BL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_BR: return "Unarmed_Combat_Crouch_Walk_BR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_F:  return "Unarmed_Combat_Crouch_Walk_F";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_FL: return "Unarmed_Combat_Crouch_Walk_FL";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_FR: return "Unarmed_Combat_Crouch_Walk_FR";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_L:  return "Unarmed_Combat_Crouch_Walk_L";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_R:  return "Unarmed_Combat_Crouch_Walk_R";
            
    //Unarmed_Locomotion_FPP
    //- �ޱ׷��� �����Ѵ�(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_F_FPP:  return "Unarmed_Combat_Crouch_Sprint_F_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FL_FPP: return "Unarmed_Combat_Crouch_Sprint_FL_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FR_FPP: return "Unarmed_Combat_Crouch_Sprint_FR_FPP";
    //- ������� �޸���(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_B_FPP:  return "Unarmed_Combat_Prone_Run_B_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BL_FPP: return "Unarmed_Combat_Prone_Run_BL_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BR_FPP: return "Unarmed_Combat_Prone_Run_BR_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_F_FPP:  return "Unarmed_Combat_Prone_Run_F_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FL_FPP: return "Unarmed_Combat_Prone_Run_FL_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FR_FPP: return "Unarmed_Combat_Prone_Run_FR_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_L_FPP:  return "Unarmed_Combat_Prone_Run_L_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_R_FPP:  return "Unarmed_Combat_Prone_Run_R_FPP";
    //- ������� �ȴ´�(8)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_B_FPP:  return "Unarmed_Combat_Prone_Walk_B_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BL_FPP: return "Unarmed_Combat_Prone_Walk_BL_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BR_FPP: return "Unarmed_Combat_Prone_Walk_BR_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_F_FPP:  return "Unarmed_Combat_Prone_Walk_F_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FL_FPP: return "Unarmed_Combat_Prone_Walk_FL_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FR_FPP: return "Unarmed_Combat_Prone_Walk_FR_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_L_FPP:  return "Unarmed_Combat_Prone_Walk_L_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_R_FPP:  return "Unarmed_Combat_Prone_Walk_R_FPP";
    //- ���� �����Ѵ�(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F_FPP:  return "Unarmed_Combat_Stand_Sprint_F_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL_FPP: return "Unarmed_Combat_Stand_Sprint_FL_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR_FPP: return "Unarmed_Combat_Stand_Sprint_FR_FPP";
            
    //Unarmed_Landing(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Fall_Landing_Additive: return "Unarmed_Combat_Fall_Landing_Additive"; //������ ����
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Fall_Landing_Extreme:  return "Unarmed_Combat_Fall_Landing_Extreme";  //�����ϴ� �Ѿ�������
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Fall_Landing_Hard:     return "Unarmed_Combat_Fall_Landing_Hard";     //���ϴ� ����
            
    //Unarmed_Jump(2)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F:          return "Unarmed_Combat_Jump_F";          //������ ����
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary: return "Unarmed_Combat_Jump_Stationary"; //���ڸ� ����
            
    //Unarmed_Jump_FPP(2)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F_FPP:          return "Unarmed_Combat_Jump_F_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary_FPP: return "Unarmed_Combat_Jump_Stationary_FPP";
            
    //Unarmed_Idling(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1: return "Unarmed_Combat_Crouch_Idling_1";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1:  return "Unarmed_Combat_Prone_Idling_1";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1:  return "Unarmed_Combat_Stand_Idling_1";
            
    //Unarmed_DoorOpen_And_Pickup(6)
    //- ������(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_DoorOpen: return "Unarmed_Combat_Crouch_DoorOpen";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_DoorOpen:  return "Unarmed_Combat_Prone_DoorOpen";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_DoorOpen:  return "Unarmed_Combat_Stand_DoorOpen";
            
    //- �ٴڿ� �ִ� �����ݱ�(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Pickup_Low: return "Unarmed_Combat_Crouch_Pickup_Low";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Pickup_Low:  return "Unarmed_Combat_Prone_Pickup_Low";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Pickup_Low:  return "Unarmed_Combat_Stand_Pickup_Low";
            
    //Unarmed_Pickup_FPP(3)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Pickup_FPP: return "Unarmed_Combat_Crouch_Pickup_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Pickup_FPP:  return "Unarmed_Combat_Prone_Pickup_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Pickup_FPP:  return "Unarmed_Combat_Stand_Pickup_FPP";
            
    //Unarmed_Attack(7)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_1: return "Unarmed_Combat_Crouch_Attack_1"; //�ޱ׸�ä �����ָ�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_2: return "Unarmed_Combat_Crouch_Attack_2"; //�ޱ׸�ä ���� �޼��ָ�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_3: return "Unarmed_Combat_Crouch_Attack_3"; //�ޱ׸�ä ������ �޼��ָ�
            
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Fullbody_PowerAttack_1: return "Unarmed_Combat_Fullbody_PowerAttack_1"; //������ġ
            
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_1: return "Unarmed_Combat_Upperbody_Attack_1"; //���� �����ָ�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_2: return "Unarmed_Combat_Upperbody_Attack_2"; //���� ���� �޼��ָ�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_3: return "Unarmed_Combat_Upperbody_Attack_3"; //���� ������ �޼��ָ�
            
    //Unarmed_Attack_FPP(6)
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_1_FPP: return "Unarmed_Combat_Crouch_Attack_1_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_2_FPP: return "Unarmed_Combat_Crouch_Attack_2_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_3_FPP: return "Unarmed_Combat_Crouch_Attack_3_FPP";
            
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_1_FPP: return "Unarmed_Combat_Upperbody_Attack_1_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_2_FPP: return "Unarmed_Combat_Upperbody_Attack_2_FPP";
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_3_FPP: return "Unarmed_Combat_Upperbody_Attack_3_FPP";

    //3. ���⸦ ������� �� �ִϸ��̼�
    //Rifle_Transition(10)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Aim_Stand:  return "Rifle_Combat_Crouch_Aim_Stand";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Prone: return "Rifle_Combat_Crouch_Base_Prone";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Stand: return "Rifle_Combat_Crouch_Base_Stand";

    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Crouch: return "Rifle_Combat_Prone_Base_Crouch";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Stand:  return "Rifle_Combat_Prone_Base_Stand";

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Aim_Crouch:     return "Rifle_Combat_Stand_Aim_Crouch";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Crouch:    return "Rifle_Combat_Stand_Base_Crouch";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Prone:     return "Rifle_Combat_Stand_Base_Prone";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_F_Prone:    return "Rifle_Combat_Stand_Run_F_Prone";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_Prone: return "Rifle_Combat_Stand_Sprint_F_Prone";

    //Rifle_OnHand(4)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand:   return "Rifle_Combat_Stand_PrimarySlot_OnHand";   //���� �ֹ��⸦ �տ� ���
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand: return "Rifle_Combat_Stand_SecondarySlot_OnHand"; //���� �������⸦ �տ� ���
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand:   return "Rifle_Combat_Prone_PrimarySlot_OnHand";   //���帰ä �ֹ��⸦ �տ� ���
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand: return "Rifle_Combat_Prone_SecondarySlot_OnHand"; //���帰ä �������⸦ �տ� ���

    //Rifle_OnBody(4)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot:          return "Rifle_Combat_Prone_PrimarySlot";          //����� �ֹ��⸦ � �Ǵ�
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot:        return "Rifle_Combat_Prone_SecondarySlot";        //����� �������⸦ � �Ǵ�
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static:   return "Rifle_Combat_Stand_PrimarySlot_Static";   //���� �ֹ��⸦ � �Ǵ�
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static: return "Rifle_Combat_Stand_SecondarySlot_Static"; //���� �������⸦ � �Ǵ�

    //Rifle_Locomotion_Stand
    //- ���� �޸���(8)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_B:  return "Rifle_Combat_Stand_Run_B";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BL: return "Rifle_Combat_Stand_Run_BL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BR: return "Rifle_Combat_Stand_Run_BR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_F:  return "Rifle_Combat_Stand_Run_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FL: return "Rifle_Combat_Stand_Run_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FR: return "Rifle_Combat_Stand_Run_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_L:  return "Rifle_Combat_Stand_Run_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_R:  return "Rifle_Combat_Stand_Run_R";

    //- ���� �����Ѵ�(13)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponDown:   return "Rifle_Combat_Stand_Sprint_F_WeaponDown";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponInView: return "Rifle_Combat_Stand_Sprint_F_WeaponInView";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponUp:     return "Rifle_Combat_Stand_Sprint_F_WeaponUp";

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_BigGun:     return "Rifle_Combat_Stand_Sprint_FL_BigGun";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponDown: return "Rifle_Combat_Stand_Sprint_FL_WeaponDown";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponUp:   return "Rifle_Combat_Stand_Sprint_FL_WeaponUp";

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_BigGun:     return "Rifle_Combat_Stand_Sprint_FR_BigGun";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponDown: return "Rifle_Combat_Stand_Sprint_FR_WeaponDown";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponUp:   return "Rifle_Combat_Stand_Sprint_FR_WeaponUp";

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_L_WeaponDown: return "Rifle_Combat_Stand_Sprint_L_WeaponDown";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_L_WeaponUp:   return "Rifle_Combat_Stand_Sprint_L_WeaponUp";

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_R_WeaponDown: return "Rifle_Combat_Stand_Sprint_R_WeaponDown";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_R_WeaponUp:   return "Rifle_Combat_Stand_Sprint_R_WeaponUp";

    //- ���� �ȴ´�(8)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_B:  return "Rifle_Combat_Stand_Walk_B";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_BL: return "Rifle_Combat_Stand_Walk_BL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_BR: return "Rifle_Combat_Stand_Walk_BR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_F:  return "Rifle_Combat_Stand_Walk_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_FL: return "Rifle_Combat_Stand_Walk_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_FR: return "Rifle_Combat_Stand_Walk_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_L:  return "Rifle_Combat_Stand_Walk_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_R:  return "Rifle_Combat_Stand_Walk_R";

    //Rifle_Locomotion_Prone(15)
    //- ������� �޸���(8)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_B:  return "Rifle_Combat_Prone_Run_B";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_BL: return "Rifle_Combat_Prone_Run_BL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_BR: return "Rifle_Combat_Prone_Run_BR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_F:  return "Rifle_Combat_Prone_Run_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_FL: return "Rifle_Combat_Prone_Run_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_FR: return "Rifle_Combat_Prone_Run_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_L:  return "Rifle_Combat_Prone_Run_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_R:  return "Rifle_Combat_Prone_Run_R";

    //- ������� �ȴ´�(7)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_BL: return "Rifle_Combat_Prone_Walk_BL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_BR: return "Rifle_Combat_Prone_Walk_BR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_F:  return "Rifle_Combat_Prone_Walk_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_FL: return "Rifle_Combat_Prone_Walk_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_FR: return "Rifle_Combat_Prone_Walk_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_L:  return "Rifle_Combat_Prone_Walk_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_R:  return "Rifle_Combat_Prone_Walk_R";

    //Rifle_Locomotion_Crouch
    //- �ޱ׸�ä �޸���(8)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_B:  return "Rifle_Combat_Crouch_Run_B";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BL: return "Rifle_Combat_Crouch_Run_BL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BR: return "Rifle_Combat_Crouch_Run_BR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_F:  return "Rifle_Combat_Crouch_Run_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_FL: return "Rifle_Combat_Crouch_Run_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_FR: return "Rifle_Combat_Crouch_Run_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_L:  return "Rifle_Combat_Crouch_Run_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_R:  return "Rifle_Combat_Crouch_Run_R";

    //- �ޱ׸�ä �����Ѵ�(5)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_F:  return "Rifle_Combat_Crouch_Sprint_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FL: return "Rifle_Combat_Crouch_Sprint_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FR: return "Rifle_Combat_Crouch_Sprint_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_L:  return "Rifle_Combat_Crouch_Sprint_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_R:  return "Rifle_Combat_Crouch_Sprint_R";

    //- �ޱ׸�ä �ȴ´�(8)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_B:  return "Rifle_Combat_Crouch_Walk_B";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_BL: return "Rifle_Combat_Crouch_Walk_BL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_BR: return "Rifle_Combat_Crouch_Walk_BR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_F:  return "Rifle_Combat_Crouch_Walk_F";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_FL: return "Rifle_Combat_Crouch_Walk_FL";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_FR: return "Rifle_Combat_Crouch_Walk_FR";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_L:  return "Rifle_Combat_Crouch_Walk_L";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_R:  return "Rifle_Combat_Crouch_Walk_R";

    //Rifle_Locomotion_FPP(9)
    //- �ޱ׸�ä �����Ѵ�(3)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_F_FPP:  return "Rifle_Combat_Crouch_Sprint_F_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FL_FPP: return "Rifle_Combat_Crouch_Sprint_FL_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FR_FPP: return "Rifle_Combat_Crouch_Sprint_FR_FPP";
    //- ���� �����Ѵ�(6)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponDown_FPP:   return "Rifle_Combat_Stand_Sprint_F_WeaponDown_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponInView_FPP: return "Rifle_Combat_Stand_Sprint_F_WeaponInView_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_BigGun_FPP:      return "Rifle_Combat_Stand_Sprint_FL_BigGun_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponDown_FPP:  return "Rifle_Combat_Stand_Sprint_FL_WeaponDown_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_BigGun_FPP:      return "Rifle_Combat_Stand_Sprint_FR_BigGun_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponDonw_FPP:  return "Rifle_Combat_Stand_Sprint_FR_WeaponDonw_FPP";

    //Rifle_Landing(2)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Fall_Landing_Extreme: return "Rifle_Combat_Fall_Landing_Extreme"; //������ �����ϴ� �Ѿ���
    case TAG_ANIM_CHARACTER::Rifle_Combat_Fall_Landing_Hard:    return "Rifle_Combat_Fall_Landing_Hard";    //������ �� ����

    //Rifle_Jump(4)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Jump_F:                   return "Rifle_Combat_Jump_F"; //������ ����
    case TAG_ANIM_CHARACTER::Rifle_Combat_Jump_B:                   return "Rifle_Combat_Jump_B"; //�ڷ� ����
    case TAG_ANIM_CHARACTER::Rifle_Combat_Jump_Stationary_Full_001: return "Rifle_Combat_Jump_Stationary_Full_001";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Jump_F_Start_Aimed:       return "Rifle_Combat_Jump_F_Start_Aimed";

    //Rifle_Idling(10)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base:           return "Rifle_Combat_Crouch_Base";           //���⸦ ��ä �ޱ׷��ִ´�(��������x)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Low_Idle_Still: return "Rifle_Combat_Crouch_Low_Idle_Still"; //���⸦ ��ä �ޱ׷��ִ´�(������)

    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base:           return "Rifle_Combat_Prone_Base";           //���⸦ ��ä ������ִ´�(��������x)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Low_Idle_Still: return "Rifle_Combat_Prone_Low_Idle_Still"; //���⸦ ��ä ������ִ´�(������)

    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Aim:                 return "Rifle_Combat_Stand_Aim";                 //����(��������x)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base:                return "Rifle_Combat_Stand_Base";                //�⺻(��������x)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle:       return "Rifle_Combat_Stand_Base_LocoIdle";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Hipfire:             return "Rifle_Combat_Stand_Hipfire";             //�����ػ��(��������x: LocoIdle�� ����ġ�� ����)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Base:            return "Rifle_Combat_Stand_Low_Base";            //�ѱ��� �Ʒ��� ����(��������x)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Idle_Still_Very: return "Rifle_Combat_Stand_Low_Idle_Still_Very"; //LocoIdle�� ���� ����ġ ����(�����δ�)

    //Rifle_DoorOpen_And_Pickup(6)
    //- ������(3)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_DoorOpen: return "Rifle_Combat_Crouch_DoorOpen";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_DoorOpen:  return "Rifle_Combat_Prone_DoorOpen";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_DoorOpen:  return "Rifle_Combat_Stand_DoorOpen";

    //- �ٴڿ� �ִ� ���� ����(3)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Pickup_Low: return "Rifle_Combat_Crouch_Pickup_Low";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Pickup_Low:  return "Rifle_Combat_Prone_Pickup_Low";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Pickup_Low:  return "Rifle_Combat_Stand_Pickup_Low";

    //Rifle_Pickup_FPP(3)
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Pickup_FPP: return "Rifle_Combat_Crouch_Pickup_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Pickup_FPP:  return "Rifle_Combat_Prone_Pickup_FPP";
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Pickup_FPP:  return "Rifle_Combat_Stand_Pickup_FPP";

    //4. ������ ����� �� �ִϸ��̼�
    //Healing(6)
    case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Aidkit:  return "Healing_Combat_Prone_Aidkit";
    case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Bandage: return "Healing_Combat_Prone_Bandage";
    case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Medkit:  return "Healing_Combat_Prone_Medkit";

    case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Aidkit:  return "Healing_Combat_Stand_Aidkit";
    case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Bandage: return "Healing_Combat_Stand_Bandage";
    case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Medkit:  return "Healing_Combat_Stand_Medkit";
    //Healing_FPP(6)
    case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Aidkit_FPP:  return "Healing_Combat_Prone_Aidkit_FPP";
    case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Bandage_FPP: return "Healing_Combat_Prone_Bandage_FPP";
    case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Medkit_FPP:  return "Healing_Combat_Prone_Medkit_FPP";

    case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Aidkit_FPP:  return "Healing_Combat_Stand_Aidkit_FPP";
    case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Bandage_FPP: return "Healing_Combat_Stand_Bandage_FPP";
    case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Medkit_FPP:  return "Healing_Combat_Stand_Medkit_FPP";

    //5. ������ ������ �ִϸ��̼�
    //DBNO(14)
    case TAG_ANIM_CHARACTER::DBNO_Enter:             return "DBNO_Enter";             //���ִٰ� �Ѹ¾��� ��
    case TAG_ANIM_CHARACTER::DBNO_Enter_From_Crouch: return "DBNO_Enter_From_Crouch"; //�ޱ׷��ִ� �Ѹ¾��� ��
    case TAG_ANIM_CHARACTER::DBNO_Enter_From_Prone:  return "DBNO_Enter_From_Prone";  //������ִ� �Ѹ¾��� ��
    case TAG_ANIM_CHARACTER::DBNO_Exit_To_Crouch:    return "DBNO_Exit_To_Crouch";    //�ޱ׷��ִ� ��ݻ��¿��� �������ö�

    case TAG_ANIM_CHARACTER::DBNO_Idle: return "DBNO_Idle";

    //�Ѹ��� ���·� ������ �� 
    case TAG_ANIM_CHARACTER::DBNO_Move_B:  return "DBNO_Move_B";
    case TAG_ANIM_CHARACTER::DBNO_Move_BL: return "DBNO_Move_BL";
    case TAG_ANIM_CHARACTER::DBNO_Move_BR: return "DBNO_Move_BR";
    case TAG_ANIM_CHARACTER::DBNO_Move_F:  return "DBNO_Move_F";
    case TAG_ANIM_CHARACTER::DBNO_Move_FL: return "DBNO_Move_FL";
    case TAG_ANIM_CHARACTER::DBNO_Move_FR: return "DBNO_Move_FR";
    case TAG_ANIM_CHARACTER::DBNO_Move_L:  return "DBNO_Move_L";
    case TAG_ANIM_CHARACTER::DBNO_Move_R:  return "DBNO_Move_R";

    //���� �һ���ų ��
    case TAG_ANIM_CHARACTER::DBNO_Revive: return "DBNO_Revive";
    //
    case TAG_ANIM_CHARACTER::COUNT: return "COUNT";
    default:
        {
            assert(false && "TagAnimToString::Get(), default case.");
            return "";
        }
    }
}

float TagAnimation::GetSpeed(const TAG_ANIM_CHARACTER tag)
{
    switch (tag)
    {
        //������� ���� �޸�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_R:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_L:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FL:
        return 1.8f;

        //������� ���� ������
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR:
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_L: //�Ⱦ�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_R: //�Ⱦ�
        return 1.4f;
        
        //���� ��� ���� �޸�
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_F:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_R:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BR:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_B:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BL:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_L:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FL:
        return 0.5f;

        //���� ���� �ޱ׸��� �޸�
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FL:
        return 0.5f;

        //���� ��� ����
    case TAG_ANIM_CHARACTER::Rifle_Combat_Fall_Landing_Hard:
        return 1.5f; //�ӵ� �ø��� �ִϸ��̼��� ó���� ����Ű�� ������

        //���� -> ���帮��
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Prone:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Prone:

        //���帮�� -> ����
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Stand:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Stand:
    
        //���帮�� -> �ޱ׸���
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Crouch:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Crouch:

        //�ޱ׸��� -> ���帮��
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Prone:
    case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Prone:
        return 1.0f; //�ӵ� �ø��� �ִϸ��̼��� ó���� ����Ű�� ������

    default:
        return 1.0f;
    }
}

float TagAnimation::GetSpeed(const TAG_ANIM_WEAPON tag)
{
    switch (tag)
    {
    case TAG_ANIM_WEAPON::Weapon_QBZ_Reload_Charge_FPP:
        return 0.83f;

    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Start:
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Loop:
    case TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_End:
        return 0.95f;

    default:
        return 1.0f;
    }
}

TAG_ANIM_CHARACTER TagAnimation::GetFPP(const TAG_ANIM_CHARACTER tag)
{
    //case TAG_ANIM_CHARACTER::
    //    return TAG_ANIM_CHARACTER::
    switch (tag)
    {
    //Weapon_Kar98k_Character_FPP.X
        {
        case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Base:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Prone:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_BoltAction_1_Prone_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Base:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Base:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Base:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Prone:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Prone_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Prone:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Prone_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Prone:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Prone_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Base:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_FPP;

        case TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Prone:
            return TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Prone_FPP;
        }

    //Weapon_QBZ_Character_FPP.X
        {
        case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Base:
            return TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_FPP;

        case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_Base:
            return TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_FPP;

        case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_Prone:
            return TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Charge_Prone_FPP;

        case TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Prone:
            return TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Prone_FPP;
        }

    //Unarmed_Locomotion_FPP.X
        {
            //������� ���� �����Ѵ�
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR_FPP;

            //������� �ޱ׷��� �����Ѵ�
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_F:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_F_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FL:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FL_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FR:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FR_FPP;

            //������� ������� �޸���
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_B:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_B_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BL:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BL_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BR:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BR_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_F:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_F_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FL:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FL_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FR:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FR_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_L:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_L_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_R:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_R_FPP;

            //������� ������� �ȴ´�
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_B:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_B_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BL:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BL_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BR:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BR_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_F:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_F_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FL:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FL_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FR:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FR_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_L:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_L_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_R:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_R_FPP;
        }

    //Unarmed_Pickup_FPP.X
        {
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Pickup_Low:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Pickup_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Pickup_Low:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Pickup_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Pickup_Low:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Pickup_FPP;
        }

    //Unarmed_Jump_FPP.X
        {
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary_FPP;
        }

    //Unarmed_Attack_FPP.X
        {
        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_1:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_1_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_2:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_2_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_3:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_3_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_1:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_1_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_2:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_2_FPP;

        case TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_3:
            return TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_3_FPP;
        }

    //Rifle_Pickup_FPP.X
        {
        case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Pickup_Low:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Pickup_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Pickup_Low:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Pickup_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Pickup_Low:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Pickup_FPP;
        }

    //Rifle_Locomotion_FPP.X
        {
        case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_F:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_F_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FL:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FL_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FR:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FR_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponDown:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponDown_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponInView:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponInView_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_BigGun:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_BigGun_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponDown:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_WeaponDown_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_BigGun:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_BigGun_FPP;

        case TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponDown:
            return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_WeaponDonw_FPP;    
        }

    //Healing_FPP.X
        {
        case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Aidkit:
            return TAG_ANIM_CHARACTER::Healing_Combat_Prone_Aidkit_FPP;

        case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Bandage:
            return TAG_ANIM_CHARACTER::Healing_Combat_Prone_Bandage_FPP;

        case TAG_ANIM_CHARACTER::Healing_Combat_Prone_Medkit:
            return TAG_ANIM_CHARACTER::Healing_Combat_Prone_Medkit_FPP;

        case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Aidkit:
            return TAG_ANIM_CHARACTER::Healing_Combat_Stand_Aidkit_FPP;

        case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Bandage:
            return TAG_ANIM_CHARACTER::Healing_Combat_Stand_Bandage_FPP;

        case TAG_ANIM_CHARACTER::Healing_Combat_Stand_Medkit:
            return TAG_ANIM_CHARACTER::Healing_Combat_Stand_Medkit_FPP;
        }

    default:
        return tag;
    }
}