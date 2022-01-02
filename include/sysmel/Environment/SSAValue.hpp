#ifndef SYSMEL_ENVIRONMENT_SSA_VALUE_HPP
#define SYSMEL_ENVIRONMENT_SSA_VALUE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSAValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAConstantValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAConstantLiteralValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGlobalValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSAProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGlobalVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAProgramEntityWithChildren);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAFunction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSACodeRegion);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSABasicBlock);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAModule);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSANamespace);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSATypeProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSATemplate);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSATemplateInstance);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACodeRegionLocalValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSACodeRegionArgument);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSACodeRegionCapture);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSABreakInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACallInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAContinueInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAConditionalJumpInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADeclareLocalVariableInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADoWhileInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADoWithCleanupInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAIfInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAJumpInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALoadInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALocalVariableInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGetAggregateFieldReferenceInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGetAggregateSlotReferenceInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAMakeClosureInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAMakeVectorInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAReturnFromFunctionInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAReturnFromRegionInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSASendMessageInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAStoreInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAUnreachableInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAVectorSwizzleInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAWhileInstruction);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACastInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAUpcastInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADowncastInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSABitcastInstruction);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAEnableLocalFinalization);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALocalFinalization);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACheckExpectedTypeSelectorValueInstruction);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAValueVisitor);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAValue : public SubtypeOf<CompilerObject, SSAValue>
{
public:
    static constexpr char const __typeName__[] = "SSAValue";

    virtual bool isSSAValue() const override;
    virtual bool isTerminatorInstruction() const;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor);

    virtual SExpression asFullDefinitionSExpression() const override;

    virtual std::string printString() const override;
    virtual std::string fullPrintString() const override;

    virtual TypePtr getValueType() const;

    virtual void replaceUsesOfWith(const SSAValuePtr &useToReplace, const SSAValuePtr &replacement);

    virtual void addUse(const SSAValuePtr &useLocation);
    virtual void replaceWith(const SSAValuePtr &replacement);

    virtual void markLocalFinalizationRequired();

protected:
    std::unordered_set<SSAValueWeakPtr> useList;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_VALUE_HPP