#pragma once
#include "TagObject.h"

class Component;
class Transform;

class IObject
{
private:
	unordered_map<type_index, Component*> m_components;
	Transform* m_pTransform;

	TAG_OBJECT m_tag;

	IObject*        m_pParent;
	deque<IObject*> m_children;

protected:
	IObject();

public:
	virtual ~IObject();

	void Update();
	void Render();

	void       SetTag(const TAG_OBJECT tag);
    TAG_OBJECT GetTag() const;

	void SetParent(IObject* pParent);
	void AddChildren(IObject* pChild);

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
			assert(false && "GameObject::GetComponent(), Component does not exist.");

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
			SAFE_DELETE(search->second);

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

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
};
