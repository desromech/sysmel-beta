#include "sysmel/BootstrapEnvironment/IdentityTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<IdentityTypeConversionRule> identityTypeConversionRuleRegistration;

TypeConversionRulePtr IdentityTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = std::make_shared<IdentityTypeConversionRule> ();
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

ASTNodePtr IdentityTypeConversionRule::convertNodeIntoWith(const ASTNodePtr &node, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)targetType;
    (void)semanticAnalyzer;
    return node;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius