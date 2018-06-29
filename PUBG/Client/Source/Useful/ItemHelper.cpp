#include "stdafx.h"
#include "ItemHelper.h"

float ItemHelper::GetDamageReduction(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Head_Lv1: return 0.7f;
        //...
    default: return 1.0f;
    }
}

float ItemHelper::GetDefaultDurability(const TAG_RES_STATIC tag)
{
    switch (tag)
    {
    case TAG_RES_STATIC::Head_Lv1: return 80.0f;
        //..
    default: 0.0f;
    }
}
