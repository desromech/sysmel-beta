#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPORARY_REFERENCE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPORARY_REFERENCE_TYPE_HPP
#pragma once

#include "PointerLikeType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceTypeValue);

/**
 * I am an instance of a function type object.
 */
class TemporaryReferenceType : public SubMetaTypeOf<PointerLikeType, TemporaryReferenceType>
{
public:
    static TemporaryReferenceTypePtr make(const TypePtr &baseType);
    static TemporaryReferenceTypePtr makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace);

    virtual bool isTemporaryReferenceType() const override;
    virtual bool isReferenceLikeType() const override;

    virtual ReferenceTypePtr ref() override;
    virtual ReferenceTypePtr refFor(const AnyValuePtr &addressSpace) override;
    virtual PointerLikeTypePtr tempRef() override;
    virtual PointerLikeTypePtr tempRefFor(const AnyValuePtr &addressSpace) override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    virtual PointerLikeTypeValuePtr makeWithValue(const AnyValuePtr &value) override;
};

/**
 * I am an instance of a function type object.
 */
class TemporaryReferenceTypeValue : public SubtypeOf<PointerLikeTypeValue, TemporaryReferenceTypeValue>
{
public:
    static constexpr char const __typeName__[] = "TemporaryReferenceType";
    static constexpr char const __sysmelTypeName__[] = "_TemporaryReferenceType";
    
    virtual bool isTemporaryReferenceTypeValue() const override;
    virtual bool isReferenceLikeTypeValue() const override;
    virtual TypePtr getType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPORARY_REFERENCE_TYPE_HPP