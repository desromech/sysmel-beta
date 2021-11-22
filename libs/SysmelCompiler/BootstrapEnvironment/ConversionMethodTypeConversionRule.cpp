#include "sysmel/BootstrapEnvironment/ConversionMethodTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/Method.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ConversionMethodTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr ConversionMethodTypeConversionRule::makeFor(const TypePtr &sourceType, const TypePtr &destinationType, const MethodPtr &method)
{
    auto rule = std::make_shared<ConversionMethodTypeConversionRule> ();
    rule->sourceType = sourceType;
    rule->destinationType = destinationType;
    rule->method = method;
    return rule;
}

bool ConversionMethodTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &nodeSourceType, const TypePtr &nodeTargetType) const
{
    (void)node;
    return nodeSourceType == sourceType && nodeTargetType == destinationType;
}

size_t ConversionMethodTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return 5;
}

ASTNodePtr ConversionMethodTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)targetType;
    assert(node->analyzedType);

    auto result = std::make_shared<ASTMessageSendNode> ();
    result->sourcePosition = sourcePosition;
    result->selector = validAnyValue(method->getName())->asASTNodeRequiredInPosition(sourcePosition);
    result->receiver = node;
    return method->analyzeMessageSendNode(result, semanticAnalyzer);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius