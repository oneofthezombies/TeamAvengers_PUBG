#pragma once

enum class Attacking
{
    Unarmed,
    Rifle,
    Melee
};

enum class Stance
{
    Stand,
    Crouch,
    Prone
};

enum class Moving
{
    Run,
    Sprint,
    Walk
};

enum class Direction
{
    Front,
    FrontRight,
    Right,
    BackRight,
    Back,
    BackLeft,
    Left,
    FrontLeft
};

struct AnimationState
{
    static TAG_ANIM_CHARACTER Get(const Attacking a, const Stance s, const Moving m, const Direction d);

private:
    static TAG_ANIM_CHARACTER get_Unarmed              (const Stance s, const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Stand        (const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Stand_Run    (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Stand_Sprint (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Stand_Walk   (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Crouch       (const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Crouch_Run   (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Crouch_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Crouch_Walk  (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Prone        (const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Prone_Run    (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Prone_Sprint (const Direction d);
    static TAG_ANIM_CHARACTER get_Unarmed_Prone_Walk   (const Direction d);

    static TAG_ANIM_CHARACTER get_Rifle(const Stance s, const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Stand(const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Stand_Run(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Stand_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Stand_Walk(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Crouch(const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Crouch_Run(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Crouch_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Crouch_Walk(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Prone(const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Prone_Run(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Prone_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Rifle_Prone_Walk(const Direction d);

    static TAG_ANIM_CHARACTER get_Melee(const Stance s, const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Stand(const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Stand_Run(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Stand_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Stand_Walk(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Crouch(const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Crouch_Run(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Crouch_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Crouch_Walk(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Prone(const Moving m, const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Prone_Run(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Prone_Sprint(const Direction d);
    static TAG_ANIM_CHARACTER get_Melee_Prone_Walk(const Direction d);
};