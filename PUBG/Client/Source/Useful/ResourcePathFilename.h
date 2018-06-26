#pragma once

struct ResourcePathFilename
{
    static pair<const string&, const string&> GetFromTag(const TAG_RESOURCE tag);
};
