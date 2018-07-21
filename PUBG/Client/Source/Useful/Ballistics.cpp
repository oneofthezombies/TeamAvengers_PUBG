#include "stdafx.h"
#include "Ballistics.h"
#include "ItemInfo.h"   
float Ballistics::GetVarianceY(const TAG_RES_STATIC tag, const float distance)
{
    switch (tag)
    {
    case TAG_RES_STATIC::QBZ:
        {
            return -1.2f;
        }
        break;
    case TAG_RES_STATIC::Kar98k:
        {
            return -0.8f;
        }
        break;
    default:
        return 0.0f;
        assert(false && "Ballistics::GetVarianceY(), default tag ");
    }
    
}
