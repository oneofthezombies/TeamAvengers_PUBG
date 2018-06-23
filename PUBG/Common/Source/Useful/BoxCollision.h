#pragma once

class BoxCollider;

struct BoxCollision
{
    static bool HasCollision(BoxCollider* pLhs, BoxCollider* pRhs);
};
