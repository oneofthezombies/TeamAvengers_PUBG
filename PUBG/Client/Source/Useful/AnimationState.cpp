#include "stdafx.h"
#include "AnimationState.h"

TAG_ANIM_CHARACTER AnimationState::Get(const Attacking a, const Stance s, const Moving m, const Direction d)
{
    switch (a) {
    case Attacking::Unarmed: return get_Unarmed(s, m, d);
    case Attacking::Rifle:   return get_Rifle(s, m, d);
    case Attacking::Melee:   return get_Melee(s, m, d);
    default: {
            assert(false && 
                "AnimationState::Get(), default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed(const Stance s, const Moving m, const Direction d)
{
    switch (s) {
    case Stance::Stand:  return get_Unarmed_Stand(m, d);
    case Stance::Crouch: return get_Unarmed_Crouch(m, d);
    case Stance::Prone:  return get_Unarmed_Prone(m, d);
    default: {
            assert(false &&
                "AnimationState::get_Unarmed(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Stand(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Unarmed_Stand_Run(d);
    case Moving::Sprint: return get_Unarmed_Stand_Sprint(d);
    case Moving::Walk:   return get_Unarmed_Stand_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Unarmed_Stand(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Stand_Run(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Stand_Sprint(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Stand_Walk(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Crouch(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Unarmed_Crouch_Run(d);
    case Moving::Sprint: return get_Unarmed_Crouch_Sprint(d);
    case Moving::Walk:   return get_Unarmed_Crouch_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Unarmed_Crouch(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Crouch_Run(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Crouch_Sprint(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Sprint_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Crouch_Walk(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Walk_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Prone(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Unarmed_Prone_Run(d);
    case Moving::Sprint: return get_Unarmed_Prone_Sprint(d);
    case Moving::Walk:   return get_Unarmed_Prone_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Unarmed_Prone(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Prone_Run(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Run_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Prone_Sprint(const Direction d)
{
    return get_Unarmed_Prone_Run(d);
}

TAG_ANIM_CHARACTER AnimationState::get_Unarmed_Prone_Walk(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Walk_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle(const Stance s, const Moving m, const Direction d)
{
    switch (s) {
    case Stance::Stand:  return get_Rifle_Stand(m, d);
    case Stance::Crouch: return get_Rifle_Crouch(m, d);
    case Stance::Prone:  return get_Rifle_Prone(m, d);
    default: {
            assert(false &&
                "AnimationState::get_Rifle(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Stand(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Rifle_Stand_Run(d);
    case Moving::Sprint: return get_Rifle_Stand_Sprint(d);
    case Moving::Walk:   return get_Rifle_Stand_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Rifle_Stand(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Stand_Run(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Idle_Still_Very;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Stand_Sprint(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_F_WeaponInView;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FR_BigGun;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_R;//Rifle_Combat_Stand_Sprint_R_WeaponDown;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Run_L;//Rifle_Combat_Stand_Sprint_L_WeaponDown;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Sprint_FL_BigGun;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Idle_Still_Very;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Stand_Walk(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Walk_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Idle_Still_Very;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Crouch(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Rifle_Crouch_Run(d);
    case Moving::Sprint: return get_Rifle_Crouch_Sprint(d);
    case Moving::Walk:   return get_Rifle_Crouch_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Rifle_Stand(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Crouch_Run(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Low_Idle_Still;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Crouch_Sprint(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Sprint_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Low_Idle_Still;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Crouch_Walk(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Walk_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Low_Idle_Still;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Prone(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Rifle_Prone_Run(d);
    case Moving::Sprint: return get_Rifle_Prone_Sprint(d);
    case Moving::Walk:   return get_Rifle_Prone_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Rifle_Stand(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Prone_Run(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_BR;
    case Direction::Back:       return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_B;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Run_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Low_Idle_Still;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Prone_Sprint(const Direction d)
{
    return get_Rifle_Prone_Run(d);
}

TAG_ANIM_CHARACTER AnimationState::get_Rifle_Prone_Walk(const Direction d)
{
    switch (d) {
    case Direction::Front:      return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_F;
    case Direction::FrontRight: return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_FR;
    case Direction::Right:      return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_R;
    case Direction::BackRight:  return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_BR;
    case Direction::BackLeft:   return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_BL;
    case Direction::Left:       return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_L;
    case Direction::FrontLeft:  return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Walk_FL;
    case Direction::StandStill: return TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Low_Idle_Still;
    default:                    return TAG_ANIM_CHARACTER::COUNT;
    }
}

//TODO: Melee생기면 추가해야할 부분
TAG_ANIM_CHARACTER AnimationState::get_Melee(const Stance s, const Moving m, const Direction d)
{
    switch (s) {
    case Stance::Stand:  return get_Melee_Stand(m, d);
    case Stance::Crouch: return get_Melee_Crouch(m, d);
    case Stance::Prone:  return get_Melee_Prone(m, d);
    default: {
            assert(false &&
                "AnimationState::get_Melee(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Stand(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Melee_Stand_Run(d);
    case Moving::Sprint: return get_Melee_Stand_Sprint(d);
    case Moving::Walk:   return get_Melee_Stand_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Melee_Stand(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Stand_Run(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Stand_Sprint(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Stand_Walk(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Crouch(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Melee_Crouch_Run(d);
    case Moving::Sprint: return get_Melee_Crouch_Sprint(d);
    case Moving::Walk:   return get_Melee_Crouch_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Melee_Crouch(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Crouch_Run(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Crouch_Sprint(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Crouch_Walk(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Prone(const Moving m, const Direction d)
{
    switch (m) {
    case Moving::Run:    return get_Melee_Prone_Run(d);
    case Moving::Sprint: return get_Melee_Prone_Sprint(d);
    case Moving::Walk:   return get_Melee_Prone_Walk(d);
    default: {
            assert(false &&
                "AnimationState::get_Melee_Prone(), \
                 default case. add special case.");
            return TAG_ANIM_CHARACTER::COUNT;
        }
    }
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Prone_Run(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Prone_Sprint(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}

TAG_ANIM_CHARACTER AnimationState::get_Melee_Prone_Walk(const Direction d)
{
    return TAG_ANIM_CHARACTER::COUNT;
}