#pragma once

enum class TAG_UI_POSITION
{
    dropped_0,
    dropped_1,
    dropped_2,
    dropped_3,
    dropped_4,
    dropped_5,
    dropped_6,
    dropped_7,
    dropped_8,
    dropped_9,
    dropped_10,
    dropped_11,
    inven_0,
    inven_1,
    inven_2,
    inven_3,
    inven_4,
    inven_5,
    inven_6,
    inven_7,
    inven_8,
    inven_9,
    inven_10,
    inven_11,
    picked,
    None,
};

struct UIPosition
{
    static bool IsDropped(const TAG_UI_POSITION tag);
    static bool IsInven(const TAG_UI_POSITION tag);
    static std::size_t GetIndex(const TAG_UI_POSITION tag);
    static TAG_UI_POSITION GetTag(
        const TAG_UI_POSITION tag, 
        const std::size_t index);
};