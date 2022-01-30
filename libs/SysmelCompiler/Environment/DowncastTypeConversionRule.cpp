#include "Environment/DowncastTypeConversionRule.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTDowncastTypeConversionNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
    auto undecoratedSourceType = sourceType->asUndecoratedType();
    auto undecoratedTargetType = targetType->asUndecoratedType();

    // Pointers and references.
    if(undecoratedSourceType->isPointerType() && undecoratedTargetType->isPointerType())
        return true;
    else if(undecoratedSourceType->isReferenceLikeType() && undecoratedTargetType->isReferenceLikeType())
        return true;
    else if(undecoratedSourceType->isReferenceLikeType() || undecoratedTargetType->isReferenceLikeType())
        return false;

    return targetType->isSubtypeOf(sourceType);
}

TypeConversionCost DowncastTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return TypeConversionCost(DirectTypeConversionCost::Downcast);
}

ASTNodePtr DowncastTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    sysmelAssert(node->analyzedType);

    auto result = basicMakeObject<ASTDowncastTypeConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel