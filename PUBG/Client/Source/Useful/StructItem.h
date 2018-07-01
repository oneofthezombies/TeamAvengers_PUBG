#pragma once
#include <string>

struct ItemTest
{
    enum class TAG_ITEM
    {
        Equipment,  //���
        Consumable, //�Ҹ�ǰ
        Ammo,       //ź��
        Weapon,     //����
        Attach      //���� ������
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

    bool                m_isDropped;   //�ٴڿ� �����ִ���
    const TAG_ITEM      m_tag;         //������ ����

    const float         m_capacity;    //�뷮
    const std::string   m_description; //������ ����
};

struct Equipment : public ItemTest
{
    const float m_damageReduction;     //���ذ��ҷ�
    const float m_durability;          //������
    const float m_capacityExtension;   //���� �뷮
};

struct Consumable : public ItemTest
{
    const float  m_castTime; //���ð�
    const float  m_recovery; //ȸ������
};

struct Ammo : public ItemTest
{
    const TAG_AMMO m_ammoType;
};

struct Weapon : public ItemTest
{
    const TAG_WEAPON m_type;          //�� ����
    const TAG_AMMO   m_useAmmoType;   //��� ź��

    const float m_baseDamage;         //�⺻ ������
    const float m_initialBulletSpeed; //ź��
    const float m_BodyHitImpactPower; //������ - ��밡 ������ ���� �� ������ ��鸮�� ����
    const float m_zeroRangeMin;       //�ּ� �����Ÿ�
    const float m_zeroRangeMax;       //�ִ� �����Ÿ�
    const float m_timeBetweenShots;   //���簣��
    const float m_duration;           //������ �ð�
    const float m_spread;             //ź����
    const float m_attachPoints;       //������������
};

struct Attach : public ItemTest
{
    float m_fasterADS;          //������ �ӵ�
    float m_magnification;      //����
};
