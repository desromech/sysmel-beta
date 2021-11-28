#include "sysmel/BootstrapEnvironment/RefCountedObject.hpp"
#include <stdlib.h>
#include <string.h>
#include <memory>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    for(auto object : allocatedObjects)
        object->setRefCountedMemoryPool(nullptr);
    for(auto object : allocatedObjects)
        object->memoryPoolForcedDestroy();
    for(auto object : allocatedObjects)
        free(object);
}

void* RefCountedObjectMemoryPool::allocateMemory(size_t size)
{
    assert(!shuttingDown);
    auto result = malloc(size);
    memset(result, 0, size);
    allocatedObjects.insert(reinterpret_cast<RefCountedObject*> (result));
    return result;
}

void RefCountedObjectMemoryPool::releaseMemoryFor(const RefCountedObject *object)
{
    auto nonConstObject = const_cast<RefCountedObject*> (object);
    free(nonConstObject);
    if(!shuttingDown)
        allocatedObjects.erase(allocatedObjects.find(nonConstObject));
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius