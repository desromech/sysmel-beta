#include "Environment/ConversionMethodTypeConversionRule.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ValueAsReceiverReferenceTypeConversionRule.hpp"
#include "Environment/Method.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ConversionMethodTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr ConversionMethodTypeConversionRule::makeFor(const TypePtr &sourceType, const TypePtr &destinationType, const MethodPtr &method)
{
    auto rule = basicMakeObject<ConversionMethodTypeConversionRule> ();
    rule->sourceType = sourceType;
    rule->destinationType = destinationType;
    rule->method = method;
    return rule;
}

bool ConversionMethodTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &nodeSourceType, const TypePtr &nodeTargetType) const
{
    bool isValidSourceType = nodeSourceType == sourceType;
    if(!isValidSourceType)
        isValidSourceType = ValueAsReceiverReferenceTypeConversionRule::uniqueInstance()->canBeUsedToConvertNodeFromTo(node, nodeSourceType, sourceType);
    return isValidSourceType && nodeTargetType == destinationType;
}

TypeConversionCost ConversionMethodTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &nodeSourceType, const TypePtr &nodeTargetType) const
{
    (void)node;
    (void)nodeSourceType;
    (void)nodeTargetType;
    return TypeConversionCost(method->costForUsingAsDirectTypeConversion());
}

ASTNodePtr ConversionMethodTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)targetType;
    sysmelAssert(node->analyzedType);

    auto result = basicMakeObject<ASTMessageSendNode> ();
    result->sourcePosition = sourcePosition;
    result->selector = validAnyValue(method->getName())->asASTNodeRequiredInPosition(sourcePosition);
    result->receiver = node;
    return method->analyzeMessageSendNode(result, semanticAnalyzer);
}

TypePtr ConversionMethodTypeConversionRule::getCanonicalSourceTypeFor(const TypePtr &conversionTargetType) const
{
    (void)conversionTargetType;
    return sourceType;
}

TypePtr ConversionMethodTypeConversionRule::getCanonicalTargetTypeFor(const TypePtr &conversionSourceType) const
{
    (void)conversionSourceType;
    return destinationType;
}

} // End of namespace Environment
} // End of namespace Sysmel