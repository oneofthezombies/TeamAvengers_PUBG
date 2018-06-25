#pragma once

class BoxCollider;

struct CollisionCore
{
    static bool HasCollision(BoxCollider* pLhs, BoxCollider* pRhs);
};
