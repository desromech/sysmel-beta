#include "Environment/RefCountedObject.hpp"
#include <stdlib.h>
#include <string.h>
#include <memory>

namespace Sysmel
{
namespace Environment
{

RefCountedObjectMemoryPool *RefCountedObjectMemoryPool::getGlobalSingleton()
{
    static auto singleton = std::make_shared<RefCountedObjectMemoryPool> ();
    return singleton.get();
}

RefCountedObjectMemoryPool::RefCountedObjectMemoryPool()
{
}

RefCountedObjectMemoryPool::~RefCountedObjectMemoryPool()
{
    assert(!shuttingDown);
    shuttingDown = true;

    // First pass: disconnect from the memory pool.
    for(auto position = firstAllocation; position; position = position->next)
    {
        auto object = reinterpret_cast<RefCountedObject*> (&position[1]);
        object->setRefCountedMemoryPool(nullptr);
    }

    // Second pass: force destroy the objects.
    for(auto position = firstAllocation; position; position = position->next)
    {
        auto object = reinterpret_cast<RefCountedObject*> (&position[1]);
        object->memoryPoolForcedDestroy();
    }

    // Third pass: free the object memory.
    for(auto position = firstAllocation; position;)
    {
        auto pointer = position;
        position = position->next;
        free(pointer);
    }
}

void* RefCountedObjectMemoryPool::allocateMemory(size_t size)
{
    assert(!shuttingDown);
    auto allocationSize = sizeof(AllocationHeader) + size;
    auto header = reinterpret_cast<AllocationHeader*> (malloc(allocationSize));
    memset(header, 0, allocationSize);

    if(lastAllocation)
        lastAllocation->next = header;
    header->previous = lastAllocation;
    lastAllocation = header;
    if(!firstAllocation)
        firstAllocation = lastAllocation;

    return &header[1];
}

void RefCountedObjectMemoryPool::releaseMemoryFor(const RefCountedObject *object)
{
    auto nonConstObject = const_cast<RefCountedObject*> (object);
    auto header = &reinterpret_cast<AllocationHeader*> (nonConstObject)[-1];

    // Update the previous side.
    if(header->previous)
        header->previous->next = header->next;
    else
        firstAllocation = header->next;

    // Update the next side.
    if(header->next)
        header->next->previous = header->previous;
    else
        lastAllocation = header->previous;

    // Free the header.
    free(header);
}

} // End of namespace Environment
} // End of namespace Sysmel