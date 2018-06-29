#pragma once
#include "TagResource.h"

struct ResourceHelper
{
    /*
        Input: Resource Tag
        Output: pair<filePath, fileName>
    */
    static pair<string, string> GetPathFileName(const TAG_RES_STATIC tag);
    static pair<string, string> GetPathFileName(const TAG_RES_ANIM_WEAPON tag);
    static pair<string, string> GetPathFileName(const TAG_RES_ANIM_CHARACTER tag);

    static bool IsItem(const TAG_RES_STATIC tag);
};
