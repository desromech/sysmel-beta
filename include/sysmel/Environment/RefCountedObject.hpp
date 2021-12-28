#ifndef SYSMEL_ENVIRONMENT_REF_COUNTED_OBJECT_HPP
#define SYSMEL_ENVIRONMENT_REF_COUNTED_OBJECT_HPP
#pragma once

#include <stdint.h>
#include <functional> // For std::hash
#include <ostream>
#include <unordered_set>
#include <utility> // For std::swap
#include "Assert.hpp"

namespace Sysmel
{
namespace Environment
{

class RefCountedObject;
class SYSMEL_COMPILER_LIB_EXPORT RefCountedObjectMemoryPool
{
public:
    struct AllocationHeader
    {
        AllocationHeader *next;
        AllocationHeader *previous;
    };

    static RefCountedObjectMemoryPool *getActive();
    static RefCountedObjectMemoryPool *getGlobalSingleton();

    RefCountedObjectMemoryPool();
    ~RefCountedObjectMemoryPool();

    void* allocateMemory(size_t size);
    void releaseMemoryFor(const RefCountedObject *object);

private:
    bool shuttingDown = false;
    AllocationHeader *firstAllocation = nullptr;
    AllocationHeader *lastAllocation = nullptr;
};

class SYSMEL_COMPILER_LIB_EXPORT RefCountedObject
{
public:
    RefCountedObject() {}
    RefCountedObject(const RefCountedObject &)
        : strongCount(1), weakCount(1) {}

    RefCountedObject(RefCountedObject &&)
        : strongCount(1), weakCount(1) {}

    virtual ~RefCountedObject()
    {
    }

    const RefCountedObject &operator=(const RefCountedObject &)
    {
        return *this;
    }

    const RefCountedObject &operator=(RefCountedObject &&)
    {
        return *this;
    }

    void strongRetain() const
    {
        sysmelAssert(strongCount > 0);
        ++strongCount;
    }

    void strongRelease() const
    {
        if(!memoryPool)
            return;

        sysmelAssert(strongCount > 0);
        --strongCount;
        if(strongCount == 0)
        {
            this->~RefCountedObject();
            weakRelease();
        }
    }

    void weakRetain() const
    {
        sysmelAssert(weakCount > 0);
        ++weakCount;
    }

    void weakRelease() const
    {
        if(!memoryPool)
            return;

        sysmelAssert(weakCount > 0);
        --weakCount;
        if(weakCount == 0)
        {
            if(memoryPool)
                memoryPool->releaseMemoryFor(this);
        }
    }

    bool weakLock() const
    {
        sysmelAssert(weakCount > 0);

        // TODO: Convert this into an atomic operation.
        if(strongCount > 0)
        {
            ++strongCount;
            return true;
        }

        return false;
    }

    void setRefCountedMemoryPool(RefCountedObjectMemoryPool *theMemoryPool)
    {
        memoryPool = theMemoryPool;
    }

    void memoryPoolForcedDestroy()
    {
        if(strongCount > 0)
            this->~RefCountedObject();
    }

private:
    mutable uint32_t strongCount = 1; // TODO: Use atomic
    mutable uint32_t weakCount = 1; // TODO: Use atomic
    RefCountedObjectMemoryPool *memoryPool;
};

template<typename T>
class ObjectPtr
{
public:
    typedef ObjectPtr<T> SelfType;

    ObjectPtr()
    {
    }

    ObjectPtr(std::nullptr_t)
    {
    }

    ObjectPtr(const SelfType &o)
        : pointer_(o.pointer_)
    {
        if(pointer_)
            reinterpret_cast<RefCountedObject*> (pointer_)->strongRetain();
    }

    ObjectPtr(SelfType &&o)
        : pointer_(o.pointer_)
    {
        o.pointer_ = nullptr;
    }

    template<typename U>
    ObjectPtr(const ObjectPtr<U> &other)
        : pointer_(other.get())
    {
        if(pointer_)
            reinterpret_cast<RefCountedObject*> (pointer_)->strongRetain();
    }

    explicit ObjectPtr(T *pointer)
        : pointer_(pointer)
    {
    }

    template<typename U>
    explicit ObjectPtr(U *pointer)
        : pointer_(pointer)
    {
    }

    ~ObjectPtr()
    {
        if(pointer_)
        {
            auto old = pointer_;
            pointer_ = nullptr;
            reinterpret_cast<RefCountedObject*> (old)->strongRelease();
        }
    }

    static SelfType fromThis(T *pointer)
    {
        reinterpret_cast<RefCountedObject*> (pointer)->strongRetain();
        return SelfType(pointer);
    }

    void swap(SelfType &other)
    {
        std::swap(pointer_, other.pointer_);
    }

    const SelfType &operator=(const SelfType &other)
    {
        if(this != &other)
            SelfType(other).swap(*this);
        return *this;
    }

    template<typename U>
    const SelfType &operator=(const ObjectPtr<U> &other)
    {
        SelfType(other).swap(*this);
        return *this;
    }

    const SelfType &operator=(SelfType &&other)
    {
        if(this != &other)
            SelfType(std::move(other)).swap(*this);
        return *this;
    }

    T *get() const
    {
        return pointer_;
    }

    T *operator->() const
    {
        return get();
    }

    T &operator*() const
    {
        return *get();
    }

    template<typename U>
    U *staticAs() const
    {
        return static_cast<U*> (pointer_);
    }

    template<typename U>
    U *dynamicAs() const
    {
        return dynamic_cast<U*> (pointer_);
    }

    operator bool() const
    {
        return pointer_;
    }

    void reset()
    {
        SelfType().swap(*this);
    }

    template<typename U>
    bool operator==(const ObjectPtr<U> &o) const
    {
        return pointer_ == o.get();
    }

    template<typename U>
    bool operator!=(const ObjectPtr<U> &o) const
    {
        return pointer_ != o.get();
    }

    template<typename U>
    bool operator<(const ObjectPtr<U> &o) const
    {
        return pointer_ < o.get();
    }

    template<typename U>
    bool operator<=(const ObjectPtr<U> &o) const
    {
        return pointer_ <= o.get();
    }

    template<typename U>
    bool operator>(const ObjectPtr<U> &o) const
    {
        return pointer_ > o.get();
    }

    template<typename U>
    bool operator>=(const ObjectPtr<U> &o) const
    {
        return pointer_ >= o.get();
    }

    friend std::ostream& operator<<(std::ostream &out, const SelfType &self)
    {
        out << self.pointer_;
        return out;
    }

private:
    T *pointer_ = nullptr;
};

template<typename T>
class ObjectWeakPtr
{
public:
    typedef ObjectWeakPtr<T> SelfType;

    ObjectWeakPtr()
    {
    }

    ObjectWeakPtr(std::nullptr_t)
    {
    }

    ObjectWeakPtr(const SelfType &o)
        : pointer_(o.pointer_)
    {
        if(pointer_)
            reinterpret_cast<RefCountedObject*> (pointer_)->weakRetain();
    }

    ObjectWeakPtr(SelfType &&o)
        : pointer_(o.pointer_)
    {
        o.pointer_ = nullptr;
    }

    template<typename U>
    ObjectWeakPtr(const ObjectWeakPtr<U> &other)
        : pointer_(other.get())
    {
        if(pointer_)
            reinterpret_cast<RefCountedObject*> (pointer_)->weakRetain();
    }

    template<typename U>
    ObjectWeakPtr(const ObjectPtr<U> &other)
        : pointer_(other.get())
    {
        if(pointer_)
            reinterpret_cast<RefCountedObject*> (pointer_)->weakRetain();
    }

    ~ObjectWeakPtr()
    {
        if(pointer_)
        {
            auto old = pointer_;
            pointer_ = nullptr;
            reinterpret_cast<RefCountedObject*> (old)->weakRelease();
        }
    }

    static SelfType fromThis(T *pointer)
    {
        reinterpret_cast<RefCountedObject*> (pointer)->weakRetain();
        return SelfType(pointer);
    }

    void swap(SelfType &other)
    {
        std::swap(pointer_, other.pointer_);
    }

    const SelfType &operator=(const SelfType &other)
    {
        if(this != &other)
            SelfType(other).swap(*this);
        return *this;
    }

    template<typename U>
    const SelfType &operator=(const ObjectPtr<U> &other)
    {
        SelfType(other).swap(*this);
        return *this;
    }

    template<typename U>
    const SelfType &operator=(const ObjectWeakPtr<U> &other)
    {
        SelfType(other).swap(*this);
        return *this;
    }

    const SelfType &operator=(SelfType &&other)
    {
        if(this != &other)
            SelfType(std::move(other)).swap(*this);
        return *this;
    }

    void reset()
    {
        SelfType().swap(*this);
    }

    ObjectPtr<T> lock() const
    {
        if(pointer_ && reinterpret_cast<RefCountedObject*> (pointer_)->weakLock())
            return ObjectPtr<T> (pointer_);
        
        return ObjectPtr<T> ();
    }

    T *getCounter() const
    {
        return pointer_;
    }

    bool operator==(const SelfType &o) const
    {
        return pointer_ == o.pointer_;
    }

    bool operator!=(const SelfType &o) const
    {
        return pointer_ != o.pointer_;
    }

    bool operator<(const SelfType &o) const
    {
        return pointer_ < o.pointer_;
    }

    bool operator<=(const SelfType &o) const
    {
        return pointer_ <= o.pointer_;
    }

    bool operator>(const SelfType &o) const
    {
        return pointer_ > o.pointer_;
    }

    bool operator>=(const SelfType &o) const
    {
        return pointer_ >= o.pointer_;
    }

private:
    T *pointer_ = nullptr;
};

template<typename T, typename...Args>
ObjectPtr<T> basicMakeObject(Args&&... args)
{
    auto pool = RefCountedObjectMemoryPool::getActive();
    auto result = reinterpret_cast<T*> (pool->allocateMemory(sizeof(T)));
    result->setRefCountedMemoryPool(pool);
    new (result) T(std::forward<Args> (args)...);
    result->setRefCountedMemoryPool(pool);
    return ObjectPtr<T>(result);
}

template<typename T, typename...Args>
ObjectPtr<T> makeObject(Args&&... args)
{
    auto result = basicMakeObject<T> (std::forward<Args> (args)...);
    result->initialize();
    return result;
}

template<typename T, typename...Args>
ObjectPtr<T> basicMakeGlobalSingletonObject(Args&&... args)
{
    auto pool = RefCountedObjectMemoryPool::getGlobalSingleton();
    auto result = reinterpret_cast<T*> (pool->allocateMemory(sizeof(T)));
    result->setRefCountedMemoryPool(pool);
    new (result) T(std::forward<Args> (args)...);
    result->setRefCountedMemoryPool(pool);
    return ObjectPtr<T>(result);
}

template<typename T, typename...Args>
ObjectPtr<T> makeGlobalSingletonObject(Args&&... args)
{
    auto result = basicMakeGlobalSingletonObject<T> (std::forward<Args> (args)...);
    result->initialize();
    return result;
}

template<typename T, typename U>
ObjectPtr<T> staticObjectCast(const ObjectPtr<U> &object)
{
    return ObjectPtr<T>::fromThis(static_cast<T*> (object.get()));
}

template<typename T, typename U>
ObjectPtr<T> dynamicObjectCast(const ObjectPtr<U> &object)
{
    return ObjectPtr<T>::fromThis(dynamic_cast<T*> (object.get()));
}

} // End of namespace Environment
} // End of namespace Sysmel

namespace std
{
template<typename T>
struct hash<Sysmel::Environment::ObjectPtr<T>>
{
    size_t operator()(const Sysmel::Environment::ObjectPtr<T> &ptr) const
    {
        return std::hash<T*> ()(ptr.get());
    }
};

template<typename T>
struct hash<Sysmel::Environment::ObjectWeakPtr<T>>
{
    size_t operator()(const Sysmel::Environment::ObjectWeakPtr<T> &ptr) const
    {
        return std::hash<T*> ()(ptr.getCounter());
    }
};
} // End of namespace std

#endif //SYSMEL_ENVIRONMENT_ANY_VALUE_HPP