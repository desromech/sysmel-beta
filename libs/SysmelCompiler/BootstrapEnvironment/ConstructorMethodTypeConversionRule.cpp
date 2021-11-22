#include "sysmel/BootstrapEnvironment/ConstructorMethodTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/Method.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ConstructorMethodTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr ConstructorMethodTypeConversionRule::makeFor(const TypePtr &sourceType, const TypePtr &destinationType, const MethodPtr &method)
{
    auto rule = std::make_shared<ConstructorMethodTypeConversionRule> ();
    rule->sourceType = sourceType;
    rule->destinationType = destinationType;
    rule->method = method;
    return rule;
}

bool ConstructorMethodTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &nodeSourceType, const TypePtr &nodeTargetType) const
{
    (void)node;
    return nodeSourceType == sourceType && nodeTargetType == destinationType;
}

size_t ConstructorMethodTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return 5;
}

ASTNodePtr ConstructorMethodTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    assert(node->analyzedType);

    auto sendNode = std::make_shared<ASTMessageSendNode> ();
    sendNode->sourcePosition = sourcePosition;
    sendNode->selector = validAnyValue(method->getName())->asASTNodeRequiredInPosition(sourcePosition);
    sendNode->receiver = targetType->asASTNodeRequiredInPosition(sourcePosition);
    sendNode->receiver->analyzedType = targetType->getType();
    sendNode->arguments = {node};
    return method->analyzeMessageSendNode(sendNode, semanticAnalyzer);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius