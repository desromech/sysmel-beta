#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_POINTER_LIKE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_POINTER_LIKE_TYPE_HPP
#pragma once

#include "DerivedType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

    virtual bool isNullableType() const override;
    virtual bool isImmutableType() const override;
    virtual bool hasTrivialInitialization() const override;
    virtual bool hasTrivialFinalization() const override;
    virtual bool hasTrivialCopyingFrom() const override;
    virtual bool hasTrivialMovingFrom() const override;

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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_POINTER_LIKE_TYPE_HPP