#pragma once
#include "IObject.h"

class Transform;

class Component
{
private:
    IObject * m_pOwner;

protected:
    Component(IObject* pOwner);

public:
    virtual ~Component();

    IObject* GetOwner() const;
	Transform* GetOwnerTransform() const;

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponent()
	{
		return m_pOwner->GetComponent<T>();
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInChildren()
	{
		return m_pOwner->GetComponentInChildren<T>();
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	vector<T*> GetComponentsInChildren()
	{
		return m_pOwner->GetComponentsInChildren();
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInParent()
	{
		return m_pOwner->GetComponentInParent();
	}
};

class Transform : public Component
{
private:
	D3DXVECTOR3    m_Position;
	D3DXQUATERNION m_Rotation;
	D3DXVECTOR3    m_Scale;
    D3DXMATRIX     m_Transformation;

public:
	Transform(IObject* pOwner);
	virtual ~Transform();

    void SetTransformationMatrix(const D3DXMATRIX& transform);
    const D3DXMATRIX& GetTransformationMatrix() const;
};