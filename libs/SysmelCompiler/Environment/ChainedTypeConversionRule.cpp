#include "Environment/ChainedTypeConversionRule.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ChainedTypeConversionRule> ChainedTypeConversionRuleRegistration;

bool ChainedTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    return firstConversionRule->canBeUsedToConvertNodeFromTo(node, sourceType, intermediateType)
        && secondConversionRule->canBeUsedToConvertNodeFromTo(node, intermediateType, sourceType);
}

TypeConversionCost ChainedTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    auto firstCost = firstConversionRule->getConversionCost(node, sourceType, intermediateType);
    auto secondCost = secondConversionRule->getConversionCost(node, intermediateType, targetType);
    return TypeConversionCost{
        uint8_t(firstCost.directCost) > uint8_t(secondCost.directCost) ? firstCost.directCost : secondCost.directCost,
        firstCost.chainLength + secondCost.chainLength + 1
    };
}

ASTNodePtr ChainedTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    auto intermediateNode = firstConversionRule->convertNodeAtIntoWith(node, sourcePosition, intermediateType, semanticAnalyzer);
    return secondConversionRule->convertNodeAtIntoWith(intermediateNode, sourcePosition, targetType, semanticAnalyzer);
}

TypePtr ChainedTypeConversionRule::getCanonicalTargetTypeFor(const TypePtr &sourceType) const
{
    (void)sourceType;
    return secondConversionRule->getCanonicalTargetTypeFor(intermediateType);
}

} // End of namespace Environment
} // End of namespace Sysmel