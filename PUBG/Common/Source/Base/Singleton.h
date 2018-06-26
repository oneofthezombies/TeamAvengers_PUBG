#pragma once

template<typename T>
class Singleton
{
protected:
    Singleton() = default;

public:
    virtual ~Singleton() = default;

    static T* GetInstance()
    {
        static T s_instance;
        return &s_instance;
    }
};