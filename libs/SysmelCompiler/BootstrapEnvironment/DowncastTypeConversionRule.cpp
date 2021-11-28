#include "sysmel/BootstrapEnvironment/DowncastTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTDowncastTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<DowncastTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr DowncastTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<DowncastTypeConversionRule> ();
    return singleton;
}

bool DowncastTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    return targetType->isSubtypeOf(sourceType);
}

size_t DowncastTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return 1;
}

ASTNodePtr DowncastTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    assert(node->analyzedType);

    auto result = basicMakeObject<ASTDowncastTypeConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius