#include "stdafx.h"
#include "Ballistics.h"
#include "ItemInfo.h"   
float Ballistics::GetVarianceY(const TAG_RES_STATIC tag, const float distance)
{
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ:
        {
        return ItemInfo;
        }
        break;
    case TAG_RES_STATIC::Kar98k:
        {

        }
        break;
    default:
        assert(false && "Ballistics::GetVarianceY(), default tag ");
    }
}
