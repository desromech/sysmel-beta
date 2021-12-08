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
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialCopyingFrom() override;
    virtual bool hasTrivialMovingFrom() override;

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
    static constexpr bool __isImmutableType__ = false;
    static constexpr bool __hasTrivialInitialization__ = true;
    static constexpr bool __hasTrivialFinalization__ = true;
    static constexpr bool __hasTrivialCopyingFrom__ = true;
    static constexpr bool __hasTrivialMovingFrom__ = true;

    virtual bool isPointerLikeTypeValue() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_POINTER_LIKE_TYPE_HPP