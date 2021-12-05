#include "sysmel/BootstrapEnvironment/FieldVariable.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTFieldVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius