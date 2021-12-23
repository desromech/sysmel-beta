#include "Environment/ConstructorMethodTypeConversionRule.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/Method.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ConstructorMethodTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr ConstructorMethodTypeConversionRule::makeFor(const TypePtr &sourceType, const TypePtr &destinationType, const MethodPtr &method)
{
    auto rule = basicMakeObject<ConstructorMethodTypeConversionRule> ();
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
    sysmelAssert(node->analyzedType);

    auto sendNode = basicMakeObject<ASTMessageSendNode> ();
    sendNode->sourcePosition = sourcePosition;
    sendNode->selector = validAnyValue(method->getName())->asASTNodeRequiredInPosition(sourcePosition);
    sendNode->receiver = targetType->asASTNodeRequiredInPosition(sourcePosition);
    sendNode->receiver->analyzedType = targetType->getType();
    sendNode->arguments = {node};
    return method->analyzeMessageSendNode(sendNode, semanticAnalyzer);
}

} // End of namespace Environment
} // End of namespace Sysmel