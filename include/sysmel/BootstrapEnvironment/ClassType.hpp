#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_TYPE_HPP
#pragma once

#include "AggregateTypeWithFields.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassTypeValue);

/**
 * I am an instance of a function type object.
 */
class ClassType : public SubMetaTypeOf<AggregateTypeWithFields, ClassType>
{
public:
    virtual bool isClassType() const override;

    virtual TypePtr getSupertype() const override;

    /// This method enqueue the analysis of a body block.
    virtual void enqueuePendingSuperclassCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);

protected:
    /// This method evaluates all of the pending code fragments.
    virtual void evaluateAllPendingCodeFragments() override;

    /// This method evaluates all of the pending body block code fragments.
    virtual void evaluateAllPendingBodyBlockCodeFragments() override;

    /// This method evaluates all of the pending super class definition code fragments.
    virtual void evaluatePendingSuperclassDefinitions() const;

    mutable DeferredCompileTimeCodeFragmentPtrList pendingSuperclassCodeFragments;
    mutable bool hasEvaluatedSuperclassCodeFragment = false;
};

/**
 * I am an instance of a function type object.
 */
class ClassTypeValue : public SubtypeOf<AggregateTypeWithFieldsValue, ClassTypeValue>
{
public:
    static constexpr char const __typeName__[] = "ClassType";
    static constexpr char const __sysmelTypeName__[] = "_ClassType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isClassTypeValue() const override;

    AnyValuePtrList slots;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_TYPE_HPP