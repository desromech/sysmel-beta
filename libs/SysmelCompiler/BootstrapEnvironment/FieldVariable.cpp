#include "sysmel/BootstrapEnvironment/FieldVariable.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTFieldVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


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