#include "sysmel/BootstrapEnvironment/TypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<TypeConversionRule> typeConversionRuleRegistration;

bool TypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    (void)sourceType;
    (void)targetType;
    return false;
}

size_t TypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius