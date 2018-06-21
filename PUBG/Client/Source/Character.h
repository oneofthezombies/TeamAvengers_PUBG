#pragma once
#include "IObject.h"
#include "Collider.h"

class MeshFilter;
class Animator;
class SkinnedMeshRenderer;
class CharacterCollisionListener;

class Character : public IObject
{
private:
    MeshFilter* pMeshFilter;
    Animator* pAnimator;
    SkinnedMeshRenderer* pSkinnedMeshRenderer;
    Frame* pWaist;
    float m_RotationX;
    CharacterCollisionListener* pCollisionListener;
    Frame* pRoot;
    float m_TranslationX;

public:
    Character();
    virtual ~Character();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};

class CharacterCollisionListener : public ICollisionListener
{
public:
    CharacterCollisionListener(IObject* pOwner);
    virtual ~CharacterCollisionListener();

    virtual void OnCollisionEnter(const Collider& other) override;
    virtual void OnCollisionExit(const Collider& other) override;
    virtual void OnCollisionStay(const Collider& other) override;
};