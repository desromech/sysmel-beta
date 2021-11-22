#include "sysmel/BootstrapEnvironment/UpcastTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTUpcastTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<UpcastTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr UpcastTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = std::make_shared<UpcastTypeConversionRule> ();
    return singleton;
}

bool UpcastTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    return sourceType->isSubtypeOf(targetType);
}

size_t UpcastTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return 1;
}

ASTNodePtr UpcastTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    assert(node->analyzedType);

    auto result = std::make_shared<ASTUpcastTypeConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius