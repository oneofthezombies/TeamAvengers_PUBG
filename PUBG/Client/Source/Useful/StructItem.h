#pragma once
#include <string>

struct ItemTest
{
    enum class TAG_ITEM
    {
        Equipment,  //장비
        Consumable, //소모품
        Ammo,       //탄약
        Weapon,     //무기
        Attach      //무기 부착물
    };

    enum class TAG_AMMO
    {
        Ammo_5_56mm,
        Ammo_7_62mm
    };

    enum class TAG_WEAPON
    {
        AR,
        SR
    };

    bool                m_isDropped;   //바닥에 놓여있는지
    const TAG_ITEM      m_tag;         //아이템 종류

    const float         m_capacity;    //용량
    const std::string   m_description; //아이템 설명
};

struct Equipment : public ItemTest
{
    const float m_damageReduction;     //피해감소량
    const float m_durability;          //내구도
    const float m_capacityExtension;   //소지 용량
};

struct Consumable : public ItemTest
{
    const float  m_castTime; //사용시간
    const float  m_recovery; //회복정도
};

struct Ammo : public ItemTest
{
    const TAG_AMMO m_ammoType;
};

struct Weapon : public ItemTest
{
    const TAG_WEAPON m_type;          //총 종류
    const TAG_AMMO   m_useAmmoType;   //사용 탄약

    const float m_baseDamage;         //기본 데미지
    const float m_initialBulletSpeed; //탄속
    const float m_BodyHitImpactPower; //저지력 - 상대가 공격을 받을 때 에임이 흔들리는 정도
    const float m_zeroRangeMin;       //최소 영점거리
    const float m_zeroRangeMax;       //최대 영점거리
    const float m_timeBetweenShots;   //연사간격
    const float m_duration;           //재장전 시간
    const float m_spread;             //탄퍼짐
    const float m_attachPoints;       //부착가능지점
};

struct Attach : public ItemTest
{
    float m_fasterADS;          //정조준 속도
    float m_magnification;      //배율
};
