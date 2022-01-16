#include "Environment/FieldVariable.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTVariableAccessNode.hpp"
#include "Environment/ASTFieldVariableAccessNode.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<FieldVariable> fieldVariableTypeRegistration;
static BootstrapTypeRegistration<FieldVariableWithReceiverVariable> fieldVariableWithReceiverVariableTypeRegistration;

bool FieldVariable::isFieldVariable() const
{
    return true;
}

AnyValuePtr FieldVariable::asMemberBoundWithReceiverVariable(const VariablePtr &receiverVariable)
{
    auto result = basicMakeObject<FieldVariableWithReceiverVariable> ();
    result->receiverVariable = receiverVariable;
    result->fieldVariable = selfFromThis();
    return result;
}

void FieldVariable::addPublicInstanceAccessingMethodsWithSymbolOnto(const AnyValuePtr &symbol, const TypePtr &type)
{
    auto self = selfFromThis();
    auto getterSelector = symbol;
    auto setterSelector = internSymbol(symbol->unwrapAsString() + ":");

    type->addMacroMethodCategories({{"accessing", {
        makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> (getterSelector, [=](const MacroInvocationContextPtr &macroContext) {
            return macroContext->astBuilder->fieldVariableAccess(macroContext->receiverNode, self);
        }, MethodFlags::Macro),

        makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> (setterSelector, [=](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &value) {
            auto &builder = macroContext->astBuilder;
            return
                builder->sendToWithArguments(builder->literalSymbol(":="),
                    builder->fieldVariableAccess(macroContext->receiverNode, self),
                    {value});
        }, MethodFlags::Macro),
    }}});
}

SExpression FieldVariable::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"fieldVariable"},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression FieldVariable::asFullDefinitionSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"fieldVariable"},
        validAnyValue(name)->asSExpression(),
        valueType->asSExpression(),
        LargeInteger{slotIndex}
    }};
}

AnyValuePtr FieldVariable::getBootstrapFieldReferenceForWithType(const AnyValuePtr &, const TypePtr &)
{
    SysmelSelfSubclassResponsibility();
}

uint32_t FieldVariable::getSlotIndex() const
{
    return slotIndex;
}

void FieldVariable::setSlotIndex(uint32_t newSlotIndex)
{
    slotIndex = newSlotIndex;
}

uint64_t FieldVariable::getOffset() const
{
    return offset;
}

void FieldVariable::setOffset(uint64_t newOffset)
{
    offset = newOffset;
}

ASTNodePtr FieldVariableWithReceiverVariable::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto receiverAccess = basicMakeObject<ASTVariableAccessNode> ();
    receiverAccess->sourcePosition = node->sourcePosition;
    receiverAccess->variable = receiverVariable;

    auto fieldAccess = basicMakeObject<ASTFieldVariableAccessNode> ();
    fieldAccess->sourcePosition = node->sourcePosition;
    fieldAccess->aggregate = receiverAccess;
    fieldAccess->fieldVariable = fieldVariable;
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(fieldAccess);
}

} // End of namespace Environment
} // End of namespace Sysmel