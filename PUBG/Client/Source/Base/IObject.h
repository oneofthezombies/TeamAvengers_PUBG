#pragma once
#include "TagClientOnly.h"

class Component;
class Transform;

class IObject : public MemoryAllocator
{
private:
	unordered_map<type_index, Component*> m_components;
    deque<IObject*>                       m_children;
    TAG_OBJECT                            m_tagObject;

    Transform* pTransform;
    IObject*   pParent;

protected:
    BoundingSphere           m_boundingSphere;
    std::vector<BoundingBox> m_boundingBoxes;

    IObject(const TAG_OBJECT tagObject);

public:
	virtual ~IObject();

	void Update();
	void Render();

	void       SetTagObject(const TAG_OBJECT tag);
    TAG_OBJECT GetTagObject() const;

	void     SetParent(IObject* pParent);
    IObject* GetParent() const;
	void AddChild(IObject* pChild);

	              Transform*                GetTransform();
            const BoundingSphere&           GetBoundingSphere();
    virtual const std::vector<BoundingBox>& GetBoundingBoxes();

    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;

    template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
    T* AddComponent()
    {
		const auto key = GetTypeIndex<T>();
		const auto search = m_components.find(key);

		T* p = nullptr;
		if (search == m_components.end())
		{
			p = new T(this);
			m_components.emplace(key, p);
		}
		return p;
    }

    template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
    T* GetComponent()
    {
		const auto key = GetTypeIndex<T>();
		const auto search = m_components.find(key);

		assert(search != m_components.end() &&
                "Object::GetComponent(), Component does not exist.");

		return static_cast<T*>(search->second);
    }

    template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
    void RemoveComponent()
    {
		const auto key = GetTypeIndex<T>();
		const auto search = m_components.find(key);

		if (search == m_components.end()) 
			return;

        SAFE_DELETE(search->second);

		m_components.erase(search);
    }

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInChildren()
	{
		for (auto& ch : m_Children)
			if (const auto co = ch->GetComponent<T>())
				return co;
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	vector<T*> GetComponentsInChildren()
	{
		vector<T*> cos;

		for (auto& ch : m_Children)
			if (const auto co = ch->GetComponent<T>())
				cos.emplace_back(co);

		return cos;
	}

	template<typename T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
	T* GetComponentInParent()
	{
		T* c = nullptr;

		if (pParent)
			c = pParent->GetComponent<T>();

		return c;
	}
};
