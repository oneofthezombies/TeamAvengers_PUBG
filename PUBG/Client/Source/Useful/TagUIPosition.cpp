#include "stdafx.h"
#include "TagUIPosition.h"

bool UIPosition::IsDropped(const TAG_UI_POSITION tag)
{
    switch (tag)
    {
    case TAG_UI_POSITION::dropped_0:
    case TAG_UI_POSITION::dropped_1:
    case TAG_UI_POSITION::dropped_2:
    case TAG_UI_POSITION::dropped_3:
    case TAG_UI_POSITION::dropped_4:
    case TAG_UI_POSITION::dropped_5:
    case TAG_UI_POSITION::dropped_6:
    case TAG_UI_POSITION::dropped_7:
    case TAG_UI_POSITION::dropped_8:
    case TAG_UI_POSITION::dropped_9:
    case TAG_UI_POSITION::dropped_10:
    case TAG_UI_POSITION::dropped_11:
        return true;
    default:
        return false;
    }
}

bool UIPosition::IsInven(const TAG_UI_POSITION tag)
{
    switch (tag)
    {
    case TAG_UI_POSITION::inven_0:
    case TAG_UI_POSITION::inven_1:
    case TAG_UI_POSITION::inven_2:
    case TAG_UI_POSITION::inven_3:
    case TAG_UI_POSITION::inven_4:
    case TAG_UI_POSITION::inven_5:
    case TAG_UI_POSITION::inven_6:
    case TAG_UI_POSITION::inven_7:
    case TAG_UI_POSITION::inven_8:
    case TAG_UI_POSITION::inven_9:
    case TAG_UI_POSITION::inven_10:
    case TAG_UI_POSITION::inven_11:
        return true;
    default:
        return false;
    }
}

std::size_t UIPosition::GetIndex(const TAG_UI_POSITION tag)
{
    switch (tag)
    {
    case TAG_UI_POSITION::dropped_0:
    case TAG_UI_POSITION::inven_0:
        return 0;
    case TAG_UI_POSITION::dropped_1:
    case TAG_UI_POSITION::inven_1:
        return 1;
    case TAG_UI_POSITION::dropped_2:
    case TAG_UI_POSITION::inven_2:
        return 2;
    case TAG_UI_POSITION::dropped_3:
    case TAG_UI_POSITION::inven_3:
        return 3;
    case TAG_UI_POSITION::dropped_4:
    case TAG_UI_POSITION::inven_4:
        return 4;
    case TAG_UI_POSITION::dropped_5:
    case TAG_UI_POSITION::inven_5:
        return 5;
    case TAG_UI_POSITION::dropped_6:
    case TAG_UI_POSITION::inven_6:
        return 6;
    case TAG_UI_POSITION::dropped_7:
    case TAG_UI_POSITION::inven_7:
        return 7;
    case TAG_UI_POSITION::dropped_8:
    case TAG_UI_POSITION::inven_8:
        return 8;
    case TAG_UI_POSITION::dropped_9:
    case TAG_UI_POSITION::inven_9:
        return 9;
    case TAG_UI_POSITION::dropped_10:
    case TAG_UI_POSITION::inven_10:
        return 10;
    case TAG_UI_POSITION::dropped_11:
    case TAG_UI_POSITION::inven_11:
        return 11;
    default:
        return std::numeric_limits<std::size_t>::max();
    }
}

TAG_UI_POSITION UIPosition::GetTag(
    const TAG_UI_POSITION tag, 
    const std::size_t index)
{
    if (IsDropped(tag))
        return static_cast<TAG_UI_POSITION>(index);

    if (IsInven(tag))
        return 
            static_cast<TAG_UI_POSITION>(
                static_cast<std::size_t>(TAG_UI_POSITION::inven_0) + index);

    return TAG_UI_POSITION::None;
}
