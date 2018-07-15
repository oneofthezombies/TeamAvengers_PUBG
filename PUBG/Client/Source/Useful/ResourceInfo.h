#pragma once
#include "TagResource.h"
#include "TagClientOnly.h"

struct ResourceInfo
{
    /*
        Input: Resource Tag
        Output: pair<filePath, fileName>
    */
    static pair<string, string> GetFileNameWithoutX(const TAG_RES_STATIC tag);
    static pair<string, string> GetPathFileName(const TAG_RES_STATIC tag);
    static pair<string, string> GetPathFileName(const TAG_RES_ANIM_WEAPON tag);
    static pair<string, string> GetPathFileName(const TAG_RES_ANIM_CHARACTER tag);
    static pair<string, string> GetPathFileName(const TAG_RES_ANIM_EQUIPMENT tag);
    static pair<string, string> GetUIPathFileName(const TAG_RES_STATIC tag);
    static pair<string, string> GetCharacterPathFileName();

    static bool IsItem(const TAG_RES_STATIC tag);

    static TAG_RES_ANIM_WEAPON GetTagResAnimWeapon(const TAG_RES_STATIC tag);

private:
    static string getPath(const TAG_RES_ANIM_EQUIPMENT tag);
    static string getFilename(const TAG_RES_ANIM_EQUIPMENT tag);
};
