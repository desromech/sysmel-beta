#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSAValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAConstantValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAConstantLiteralValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGlobalValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGlobalVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAFunction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSACodeRegion);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSABasicBlock);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACodeRegionLocalValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSACodeRegionArgument);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSABreakInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACallInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAContinueInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAConditionalJumpInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADoWhileInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADoWithCleanupInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAIfInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAJumpInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALoadInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALocalVariableInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAMakeAggregateInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAMakeClosureInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAReturnFromFunctionInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAReturnFromRegionInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSASendMessageInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAStoreInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAUnreachableInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAWhileInstruction);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACastInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAUpcastInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSADowncastInstruction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSABitcastInstruction);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAValueVisitor);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAValue : public SubtypeOf<CompilerObject, SSAValue>
{
public:
    static constexpr char const __typeName__[] = "SSAValue";

    virtual bool isSSAValue() const override;
    virtual bool isTerminatorInstruction() const;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor);

    virtual SExpression asFullSExpression() const;

    virtual std::string printString() const override;
    virtual std::string fullPrintString() const override;

    virtual TypePtr getValueType() const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_HPP