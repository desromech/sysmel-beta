#include "sysmel/BootstrapEnvironment/ValueAsVoidTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTValueAsVoidTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ValueAsVoidTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr ValueAsVoidTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = std::make_shared<ValueAsVoidTypeConversionRule> ();
    return singleton;
}

bool ValueAsVoidTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    return !sourceType->isVoidType() && targetType->isVoidType();
}

size_t ValueAsVoidTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return 0;
}

ASTNodePtr ValueAsVoidTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    assert(node->analyzedType);

    auto result = std::make_shared<ASTValueAsVoidTypeConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius