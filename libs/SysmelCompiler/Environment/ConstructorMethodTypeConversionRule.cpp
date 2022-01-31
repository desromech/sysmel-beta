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

TypeConversionCost ConstructorMethodTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    (void)sourceType;
    (void)targetType;
    return TypeConversionCost(method->costForUsingAsDirectTypeConversion());
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

TypePtr ConstructorMethodTypeConversionRule::getCanonicalSourceTypeFor(const TypePtr &targetType) const
{
    return sourceType;
}

TypePtr ConstructorMethodTypeConversionRule::getCanonicalTargetTypeFor(const TypePtr &sourceType) const
{
    return destinationType;
}

} // End of namespace Environment
} // End of namespace Sysmel