#pragma once
#include "IObject.h"
#include "TagResource.h"

class BoxCollider;

class CollidableStaticObject : public IObject
{
private:
    vector<BoxCollider*> m_colliders;

public:
             CollidableStaticObject(const TAG_RES_STATIC tag);
    virtual ~CollidableStaticObject();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

};