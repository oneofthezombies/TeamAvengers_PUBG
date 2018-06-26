#pragma once
#include "TagResource.h"

struct ResPathFileName
{
    /*
        Input: Resource Tag
        Output: pair<filePath, fileName>
    */
    static pair<string, string> Get(const TAG_RES_STATIC tag);
    static pair<string, string> Get(const TAG_RES_ANIM tag);
};