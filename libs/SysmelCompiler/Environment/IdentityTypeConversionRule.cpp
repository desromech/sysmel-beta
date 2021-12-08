#include "Environment/IdentityTypeConversionRule.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<IdentityTypeConversionRule> identityTypeConversionRuleRegistration;

TypeConversionRulePtr IdentityTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<IdentityTypeConversionRule> ();
    return singleton;
}

bool IdentityTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    return sourceType == targetType;
}

size_t IdentityTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return 0;
}

ASTNodePtr IdentityTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)sourcePosition;
    (void)targetType;
    (void)semanticAnalyzer;
    return node;
}

} // End of namespace Environment
} // End of namespace Sysmel