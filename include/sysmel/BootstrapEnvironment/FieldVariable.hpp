#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FIELD_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FIELD_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariableWithReceiverVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment);

/**
 * I am a variable program entity.
 */
class FieldVariable : public SubtypeOf<Variable, FieldVariable>
{
public:
    static constexpr char const __typeName__[] = "FieldVariable";

    virtual bool isFieldVariable() const override;
    virtual AnyValuePtr asMemberBoundWithReceiverVariable(const VariablePtr &receiverVariable) override;
    virtual void addPublicInstanceAccessingMethodsWithSymbolOnto(const AnyValuePtr &symbol, const TypePtr &type) override;

    virtual AnyValuePtr getBootstrapFieldReferenceForWithType(const AnyValuePtr &receiver, const TypePtr &referenceType);

    DeferredCompileTimeCodeFragmentPtr initialValueCodeFragment;

    uint32_t getSlotIndex() const;
    void setSlotIndex(uint32_t newSlotIndex);
    
protected:
    uint32_t slotIndex = 0;
};

/**
 * I am a variable program entity.
 */
class FieldVariableWithReceiverVariable : public SubtypeOf<CompilerObject, FieldVariableWithReceiverVariable>
{
public:
    static constexpr char const __typeName__[] = "FieldVariableWithReceiverVariable";

    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    VariablePtr receiverVariable;
    FieldVariablePtr fieldVariable;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FIELD_VARIABLE_HPP