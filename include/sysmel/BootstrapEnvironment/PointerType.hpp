#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_POINTER_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_POINTER_TYPE_HPP
#pragma once

#include "PointerLikeType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerTypeValue);

/**
 * I am an instance of a function type object.
 */
class PointerType : public SubMetaTypeOf<PointerLikeType, PointerType>
{
public:
    static PointerTypePtr make(const TypePtr &baseType);
    static PointerTypePtr makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace);

    virtual bool isPointerType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    virtual PointerLikeTypeValuePtr makeWithValue(const AnyValuePtr &value) override;
};

/**
 * I am an instance of a function type object.
 */
class PointerTypeValue : public SubtypeOf<PointerLikeTypeValue, PointerTypeValue>
{
public:
    static constexpr char const __typeName__[] = "PointerType";
    static constexpr char const __sysmelTypeName__[] = "_PointerType";

    virtual bool isPointerTypeValue() const override;
    virtual TypePtr getType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_POINTER_TYPE_HPP