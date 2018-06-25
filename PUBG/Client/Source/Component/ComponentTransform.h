#pragma once
#include "IObject.h"

class Transform;

class Component : public MemoryAllocator
{
private:
    IObject* pOwner;

protected:
    Component(IObject* pOwner);

public:
    virtual ~Component();

    IObject* GetOwner() const;
	Transform* GetTransform() const;

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponent()
	{
        assert(pOwner && "Component::GetComponent() failed. owner is null.");

		return pOwner->GetComponent<T>();
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInChildren()
	{
        assert(pOwner && 
            "Component::GetComponentInChildren() failed. owner is null.");

		return pOwner->GetComponentInChildren<T>();
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	vector<T*> GetComponentsInChildren()
	{
        assert(pOwner &&
            "Component::GetComponentsInChildren() failed. owner is null.");

		return pOwner->GetComponentsInChildren();
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInParent()
	{
        assert(pOwner &&
            "Component::GetComponentInParent() failed. owner is null.");

		return pOwner->GetComponentInParent();
	}
};

class Transform : public Component
{
private:
	D3DXVECTOR3    m_position;
	D3DXQUATERNION m_rotation;
	D3DXVECTOR3    m_scale;
    D3DXMATRIX     m_transformationMatrix;

public:
	         Transform(IObject* pOwner);
	virtual ~Transform();

    void               SetPosition(const D3DXVECTOR3& p);
    const D3DXVECTOR3& GetPosition() const;

    void                  SetRotation(const D3DXQUATERNION& r);
    const D3DXQUATERNION& GetRotation() const;

    void               SetScale(const D3DXVECTOR3& s);
    const D3DXVECTOR3& GetScale() const;

    const D3DXMATRIX& GetTransformationMatrix();
};