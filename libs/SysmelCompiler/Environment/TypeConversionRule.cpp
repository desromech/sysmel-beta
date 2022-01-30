#include "Environment/TypeConversionRule.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<TypeConversionRule> typeConversionRuleRegistration;

bool TypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    (void)sourceType;
    (void)targetType;
    return false;
}

TypeConversionCost TypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    SysmelSelfSubclassResponsibility();
}

ASTNodePtr TypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)sourcePosition;
    return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Cannot perform implicit cast from '{0}' onto '{1}'.", {node->analyzedType->printString(), targetType->printString()}));
}

} // End of namespace Environment
} // End of namespace Sysmel