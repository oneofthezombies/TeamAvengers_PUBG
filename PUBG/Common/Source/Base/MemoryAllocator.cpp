#include "stdafx.h"
#include "MemoryAllocator.h"

std::unordered_set<void*> MemoryAllocator::s_memoryAllocators;

MemoryAllocator::MemoryAllocator()
{
}

MemoryAllocator::~MemoryAllocator()
{
}

void* MemoryAllocator::operator new(std::size_t count)
{
    void* p = std::malloc(count);
    assert(p && "MemoryAllocator::operator new(), pointer is null.");
    s_memoryAllocators.emplace(p);
    return p;
}


void MemoryAllocator::operator delete(void* p)
{
    assert(p && "MemoryAllocator::operator delete(), pointer is null.");
    s_memoryAllocators.erase(p);
    std::free(p);
}

void MemoryAllocator::CheckMemoryAllocators()
{
    const auto num = s_memoryAllocators.size();
    if (num > 0)
    {
        std::string text(
            "The number of memory allocators that have not been deleted is " 
            + to_string(num));
        MessageBoxA(nullptr, text.c_str(), nullptr, MB_OK);
    }
}
