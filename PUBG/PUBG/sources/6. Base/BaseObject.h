#pragma once

class ComponentBase;

class BaseObject
{
private:
	int m_refCount;
    unordered_map<type_index, ComponentBase*> m_umapComponents;

public:
	BaseObject();
	virtual ~BaseObject();

	virtual void AddRef();
	virtual void Release();

    template<typename T, enable_if_t<is_base_of_v<ComponentBase, T>, int> = 0>
    T* SetComponent()
    {
        T* c = GetComponent<T>();
        if (!c)
        {
            c = new T(*this);
            m_umapComponents.emplace(type_index(typeid(T)), c);
        }
        return c;
    }

    template<typename T, enable_if_t<is_base_of_v<ComponentBase, T>, int> = 0>
    T* GetComponent()
    {
        const auto search = m_umapComponents.find(type_index(typeid(T)));
        if (search != m_umapComponents.end())
            return static_cast<T*>(search->second);

        return nullptr;
    }

    template<typename T, enable_if_t<is_base_of_v<ComponentBase, T>, int> = 0>
    void RemoveComponent()
    {
        auto search = m_umapComponents.find(type_index(typeid(T)));
        if (search == m_umapComponents.end()) return;

        if (search->second)
        {
            delete search->second;
            search->second = nullptr;
        }

        m_umapComponents.erase(search);
    }
};



