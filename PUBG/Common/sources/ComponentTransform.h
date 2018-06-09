#pragma once

class Object;
class Transform;

class Component
{
private:
	Object* m_pOwner;

protected:
    Component(Object* pOwner);

public:
    virtual ~Component();

    Object* GetOwner() const;
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
	D3DXVECTOR3    m_position;
	D3DXQUATERNION m_rotation;
	D3DXVECTOR3    m_scale;

public:
	Transform(Object* pOwner);
	virtual ~Transform();


};