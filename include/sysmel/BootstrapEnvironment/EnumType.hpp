#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_TYPE_HPP
#pragma once

#include "DerivedType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumTypeValue);

/**
 * I am an instance of a function type object.
 */
class EnumType : public SubMetaTypeOf<DerivedType, EnumType>
{
public:
    virtual bool isEnumType() const override;

    virtual const TypePtr &getBaseType() override;

    virtual void enqueuePendingValueTypeCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);
    virtual void enqueuePendingValuesCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);

protected:
    void evaluatePendingValueTypeCodeFragments();
    void evaluatePendingValuesFragments();

    bool hasEvaluatedValueType = false;
    DeferredCompileTimeCodeFragmentPtrList pendingValueTypeCodeFragments;
    DeferredCompileTimeCodeFragmentPtrList pendingValuesCodeFragments;
};

/**
 * I am an instance of a function type object.
 */
class EnumTypeValue : public SubtypeOf<AnyValue, EnumTypeValue>
{
public:
    static constexpr char const __typeName__[] = "EnumType";
    static constexpr char const __sysmelTypeName__[] = "_EnumType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isEnumTypeValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_TYPE_HPP