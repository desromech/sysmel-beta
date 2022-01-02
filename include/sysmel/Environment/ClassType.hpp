#ifndef SYSMEL_ENVIRONMENT_CLASS_TYPE_HPP
#define SYSMEL_ENVIRONMENT_CLASS_TYPE_HPP
#pragma once

#include "AggregateTypeWithFields.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SpecificMethod);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ClassType : public SubMetaTypeOf<AggregateTypeWithFields, ClassType>
{
public:
    virtual bool isClassType() const override;

    virtual TypePtr getSupertype() const override;

    /// This method enqueue the analysis of a body block.
    virtual void enqueuePendingSuperclassCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);

    virtual AnyValuePtr basicNewValue() override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

    virtual bool canHaveVirtualMethods() const override;
    virtual SpecificMethodPtr lookupParentOverridenMethod(const AnyValuePtr &selector, bool hasConstReceiver, const TypePtrList &argumentTypes) override;
    virtual void addVirtualMethod(const SpecificMethodPtr &virtualMethod) override;
    const SpecificMethodPtrList &getVirtualMethods() const;

protected:
    /// This method evaluates all of the pending code fragments.
    virtual void evaluateAllPendingCodeFragments() override;

    /// This method evaluates all of the pending body block code fragments.
    virtual void evaluateAllPendingBodyBlockCodeFragments() override;

    /// This method evaluates all of the pending super class definition code fragments.
    virtual void evaluatePendingSuperclassDefinitions() const;

    virtual void computeObjectLifetimeTriviality() override;

    virtual AggregateTypeLayoutPtr makeLayoutInstance() override;
    virtual void buildLayout() override;

    mutable DeferredCompileTimeCodeFragmentPtrList pendingSuperclassCodeFragments;
    mutable bool hasEvaluatedSuperclassCodeFragment = false;

    SpecificMethodPtrList virtualMethods;
    bool hasBuiltLayout = false;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ClassTypeValue : public SubtypeOf<AggregateTypeWithFieldsValue, ClassTypeValue>
{
public:
    static constexpr char const __typeName__[] = "ClassType";
    static constexpr char const __sysmelTypeName__[] = "_ClassType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isClassTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    virtual TypePtr getType() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CLASS_TYPE_HPP