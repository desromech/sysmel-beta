#ifndef SYSMEL_ENVIRONMENT_POINTER_LIKE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_POINTER_LIKE_TYPE_HPP
#pragma once

#include "DerivedType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerLikeType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerLikeTypeValue);

/**
 * I am an instance of a function type object.
 */
class PointerLikeType : public SubMetaTypeOf<DerivedType, PointerLikeType>
{
public:
    virtual bool isPointerLikeType() const override;

    virtual bool supportsDynamicCompileTimeMessageSend() const override;

    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;
    
    bool hasGenericAddressSpace() const;

    virtual PointerLikeTypeValuePtr makeWithValue(const AnyValuePtr &value);

    AnyValuePtr addressSpace;
};

/**
 * I am an instance of a function type object.
 */
class PointerLikeTypeValue : public SubtypeOf<DerivedTypeValue, PointerLikeTypeValue>
{
public:
    static constexpr char const __typeName__[] = "PointerLikeType";
    static constexpr char const __sysmelTypeName__[] = "_PointerLikeType";

    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isNullableType__ = true;
    static constexpr bool __isImmutableType__ = true;

    virtual bool isPointerLikeTypeValue() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_POINTER_LIKE_TYPE_HPP