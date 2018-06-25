#pragma once

class MemoryAllocator
{
private:
    static std::unordered_set<void*> s_memoryAllocators;

public:
    MemoryAllocator();
    virtual ~MemoryAllocator();

    void* operator new(std::size_t count);
    void operator delete(void* p);

public:
    static void CheckMemoryAllocators();
};