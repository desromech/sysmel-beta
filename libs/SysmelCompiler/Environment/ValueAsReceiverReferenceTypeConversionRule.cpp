#include "Environment/ValueAsReceiverReferenceTypeConversionRule.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ValueAsReceiverReferenceTypeConversionRule> identityTypeConversionRuleRegistration;

TypeConversionRulePtr ValueAsReceiverReferenceTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<ValueAsReceiverReferenceTypeConversionRule> ();
    return singleton;
}

bool ValueAsReceiverReferenceTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    auto undecoratedSourceType = sourceType->asUndecoratedType();
    return targetType->isReferenceType() &&
        undecoratedSourceType->isPassedByReference() &&
        undecoratedSourceType->isSubtypeOf(targetType.staticAs<ReferenceType> ()->getBaseType()->asUndecoratedType());
}

TypeConversionCost ValueAsReceiverReferenceTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return TypeConversionCost(DirectTypeConversionCost::Identity);
}

ASTNodePtr ValueAsReceiverReferenceTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    sysmelAssert(node->analyzedType);

    auto result = basicMakeObject<ASTValueAsReferenceReinterpretConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel