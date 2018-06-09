#pragma once
#include "ObjectTag.h"

class Component;
class Transform;

class Object
{
private:
	unordered_map<type_index, Component*> m_components;
	Transform* m_pTransform;

	OBJECT_TAG m_tag;

	Object*        m_pParent;
	deque<Object*> m_children;

public:
	Object();
	virtual ~Object();

	void       SetTag(const OBJECT_TAG tag);
	OBJECT_TAG GetTag() const;

	void SetParent(Object* pParent);
	void AddChildren(Object* pChild);

	Transform* GetTransform();

    template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
    T* AddComponent()
    {
		const auto key = GetTypeIndex<T>();
		const auto search = m_components.find(key);

		T* ptr = nullptr;
		if (search == m_components.end())
		{
			ptr = new T(this);
			m_components.emplace(key, ptr);
		}
		return ptr;
    }

    template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
    T* GetComponent()
    {
		const auto key = GetTypeIndex<T>();
		const auto search = m_components.find(key);

		if (search == m_components.end())
		{
			assert(false && "GameObject::GetComponent(), Component does not exist.");
		}

		return static_cast<T*>(search->second);
    }

    template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
    void RemoveComponent()
    {
		const auto key = GetTypeIndex<T>();
		const auto search = m_components.find(key);

		if (search == m_components.end()) 
			return;

		if (search->second)
		{
			SAFE_DELETE(search->second);
		}

		m_components.erase(search);
    }

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInChildren()
	{
		for (auto& ch : m_children)
			if (const auto co = ch->GetComponent<T>())
				return co;
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	vector<T*> GetComponentsInChildren()
	{
		vector<T*> coms;

		for (auto& ch : m_children)
			if (const auto co = ch->GetComponent<T>())
				coms.emplace_back(co);

		return coms;
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInParent()
	{
		T* c = nullptr;

		if (m_pParent)
			c = m_pParent->GetComponent<T>();

		return c;
	}
};
