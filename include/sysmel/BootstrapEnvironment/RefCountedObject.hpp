#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_REF_COUNTED_OBJECT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_REF_COUNTED_OBJECT_HPP
#pragma once

#include <stdint.h>
#include <assert.h>
#include <functional> // For std::hash
#include <ostream>
#include <unordered_set>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class RefCountedObject;
class RefCountedObjectMemoryPool
{
public:
    static RefCountedObjectMemoryPool *getActive();
    static RefCountedObjectMemoryPool *getGlobalSingleton();

    RefCountedObjectMemoryPool();
    ~RefCountedObjectMemoryPool();

    void* allocateMemory(size_t size);
    void releaseMemoryFor(const RefCountedObject *object);

private:
    bool shuttingDown = false;
    std::unordered_set<RefCountedObject*> allocatedObjects;
};

class RefCountedObject
{
public:
    virtual ~RefCountedObject()
    {
    }

    void strongRetain() const
    {
        assert(strongCount > 0);
        ++strongCount;
    }

    void strongRelease() const
    {
        if(!memoryPool)
            return;

        assert(strongCount > 0);
        --strongCount;
        if(strongCount == 0)
        {
            this->~RefCountedObject();
            weakRelease();
        }
    }

    void weakRetain() const
    {
        assert(weakCount > 0);
        ++weakCount;
    }

    void weakRelease() const
    {
        if(!memoryPool)
            return;

        assert(weakCount > 0);
        --weakCount;
        if(weakCount == 0)
        {
            if(memoryPool)
                memoryPool->releaseMemoryFor(this);
        }
    }

    bool weakLock() const
    {
        assert(weakCount > 0);

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

    const SelfType &operator=(const SelfType &other)
    {
        if(this != &other)
        {
            auto newPointer = other.pointer_;
            auto oldPointer = pointer_;

            if(newPointer) reinterpret_cast<RefCountedObject*> (newPointer)->strongRetain();
            pointer_ = newPointer;
            if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->strongRelease();

        }
        return *this;
    }

    template<typename U>
    const SelfType &operator=(const ObjectPtr<U> &other)
    {
        T* newPointer = other.get();
        auto oldPointer = pointer_;

        if(newPointer) reinterpret_cast<RefCountedObject*> (newPointer)->strongRetain();
        pointer_ = newPointer;
        if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->strongRelease();
        return *this;
    }

    const SelfType &operator=(SelfType &&other)
    {
        if(this != &other)
        {
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
        }
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

    operator bool() const
    {
        return pointer_;
    }

    void reset()
    {
        auto oldPointer = pointer_;
        pointer_ = nullptr;
        if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->strongRelease();
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

    const SelfType &operator=(const SelfType &other)
    {
        if(this != &other)
        {
            auto newPointer = other.pointer_;
            auto oldPointer = pointer_;

            if(newPointer) reinterpret_cast<RefCountedObject*> (newPointer)->weakRetain();
            pointer_ = newPointer;
            if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->weakRelease();

        }
        return *this;
    }

    template<typename U>
    const SelfType &operator=(const ObjectPtr<U> &other)
    {
        T* newPointer = other.get();
        auto oldPointer = pointer_;

        if(newPointer) reinterpret_cast<RefCountedObject*> (newPointer)->weakRetain();
        pointer_ = newPointer;
        if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->weakRelease();
        return *this;
    }

    template<typename U>
    const SelfType &operator=(const ObjectWeakPtr<U> &other)
    {
        T* newPointer = other.get();
        auto oldPointer = pointer_;

        if(newPointer) reinterpret_cast<RefCountedObject*> (newPointer)->weakRetain();
        pointer_ = newPointer;
        if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->weakRelease();
        return *this;
    }

    const SelfType &operator=(SelfType &&other)
    {
        if(this != &other)
        {
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
        }
        return *this;
    }

    void reset()
    {
        auto oldPointer = pointer_;
        pointer_ = nullptr;
        if(oldPointer) reinterpret_cast<RefCountedObject*> (oldPointer)->weakRelease();
    }

    ObjectPtr<T> lock() const
    {
        if(pointer_ && reinterpret_cast<RefCountedObject*> (pointer_)->weakLock())
            return ObjectPtr<T> (pointer_);
        
        return ObjectPtr<T> ();
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

namespace std
{
template<typename T>
struct hash<SysmelMoebius::BootstrapEnvironment::ObjectPtr<T>>
{
    size_t operator()(const SysmelMoebius::BootstrapEnvironment::ObjectPtr<T> &ptr) const
    {
        return std::hash<T*> ()(ptr.get());
    }
};
} // End of namespace std

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ANY_VALUE_HPP