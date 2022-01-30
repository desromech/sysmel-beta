#include "Environment/ValueAsVoidTypeConversionRule.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTValueAsVoidTypeConversionNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ValueAsVoidTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr ValueAsVoidTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<ValueAsVoidTypeConversionRule> ();
    return singleton;
}

bool ValueAsVoidTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    return !sourceType->isVoidType() && targetType->isVoidType();
}

TypeConversionCost ValueAsVoidTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return TypeConversionCost(DirectTypeConversionCost::ValueToVoid);
}

ASTNodePtr ValueAsVoidTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    sysmelAssert(node->analyzedType);

    auto result = basicMakeObject<ASTValueAsVoidTypeConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel