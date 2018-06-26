#include "stdafx.h"
#include "ResourcePathFilename.h"

pair<const string&, const string&> ResourcePathFilename::GetFromTag(
    const TAG_RESOURCE tag)
{
    static const string empty     = "";
    static const string dir_begin = "./Resource";
    static const string dir_end   =  "/";
    static const string character =  "/Character";
    static const string female    =  "/Female";
    static const string animation =  "/Animation";
    static const string model     =  "/Model";

    switch (tag)
    {
    case TAG_RESOURCE::Character_Female_Animation_DBNO:
        {
            static const string p(
                dir_begin + character + female + animation + dir_end);
            static const string f("DBNO.x");
            return make_pair(p, f);
        }
        break;
    case TAG_RESOURCE::Character_Female_Animation_Healing:
        {
            static const string p(
                dir_begin + character + female + animation + dir_end);
            static const string f("Healing.x");
            return make_pair(p, f);
        }
        break;
    default:
        {
            assert(false && 
                "ResourcePathFilename::GetFromTag(), default case.");
        }
        break;
    }

    return make_pair(empty, empty);
}
