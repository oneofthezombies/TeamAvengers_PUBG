#pragma once
#include "IObject.h"
#include "IScene.h"

class Transform;
class BoxCollider;

class SampleCollisionBox : public IObject
{
protected:
    const int m_instanceID;
    Transform*   pTransform;
    BoxCollider* pBoxCollider;

public:
             SampleCollisionBox(
                 const int instanceID, 
                 const TAG_COLLISION tagCollision, 
                 const D3DXMATRIX& transformationMatrix);
    virtual ~SampleCollisionBox();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void OnCollisionEnter(Collider* o, Collider* d);
    void OnCollisionStay(Collider* o, Collider* d);
    void OnCollisionExit(Collider* o, Collider* d);
};

class SampleControllableCollisionBox : public SampleCollisionBox
{
public:
             SampleControllableCollisionBox(
                 const int instanceID,
                 const TAG_COLLISION tagCollision,
                 const D3DXMATRIX& transformationMatrix);
    virtual ~SampleControllableCollisionBox();

    virtual void OnUpdate() override;
};

class SceneCollisionTest : public IScene
{
public:
             SceneCollisionTest();
    virtual ~SceneCollisionTest();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};