#pragma once

template<typename T>
class Singleton
{
protected:
    Singleton() = default;

public:
    ~Singleton() = default;

    static T* GetInstance()
    {
        static T instance;
        return &instance;
    }
};