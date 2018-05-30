#pragma once

template<typename T>
class SingletonBase
{
protected:
    SingletonBase() = default;

public:
    ~SingletonBase() = default;

    static T* GetInstance()
    {
        static T instance;
        return &instance;
    }
};