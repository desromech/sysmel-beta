#include "Environment/UpcastTypeConversionRule.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTUpcastTypeConversionNode.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<UpcastTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr UpcastTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<UpcastTypeConversionRule> ();
    return singleton;
}

bool UpcastTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    auto undecoratedSourceType = sourceType->asUndecoratedType();
    auto undecoratedTargetType = targetType->asUndecoratedType();

    if(undecoratedSourceType->isPointerType() && undecoratedTargetType->isPointerType())
    {
        auto sourceBaseType = undecoratedSourceType.staticAs<PointerLikeType> ()->getBaseType();
        auto targetBaseType = undecoratedTargetType.staticAs<PointerLikeType> ()->getBaseType();

        // Do not allow from const -> non-const
        if(sourceBaseType->isConstDecoratedType() && !targetBaseType->isConstDecoratedType())
            return false;

        sourceBaseType = sourceBaseType->asUndecoratedType();
        targetBaseType = targetBaseType->asUndecoratedType();
        return sourceBaseType->isSubtypeOf(targetBaseType);
    }
    else if(undecoratedSourceType->isReferenceLikeType() && undecoratedTargetType->isReferenceLikeType())
    {
        // Only allow converting from tempRef -> ref.
        if(undecoratedTargetType->isTemporaryReferenceType() && !undecoratedSourceType->isTemporaryReferenceType())
            return false;

        auto sourceBaseType = undecoratedSourceType.staticAs<PointerLikeType> ()->getBaseType();
        auto targetBaseType = undecoratedTargetType.staticAs<PointerLikeType> ()->getBaseType();

        // Do not allow from const -> non-const
        if(sourceBaseType->isConstDecoratedType() && !targetBaseType->isConstDecoratedType())
            return false;

        sourceBaseType = sourceBaseType->asUndecoratedType();
        targetBaseType = targetBaseType->asUndecoratedType();
        return sourceBaseType->isSubtypeOf(targetBaseType);
    }
    else if(undecoratedSourceType->isReferenceLikeType() || undecoratedTargetType->isReferenceLikeType())
    {
        return false;
    }

    return sourceType->isSubtypeOf(targetType);
}

TypeConversionCost UpcastTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    (void)node;
    (void)targetType;
    return TypeConversionCost(DirectTypeConversionCost::Upcast);
}

ASTNodePtr UpcastTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    sysmelAssert(node->analyzedType);

    auto result = basicMakeObject<ASTUpcastTypeConversionNode> ();
    result->sourcePosition = sourcePosition;
    result->expression = node;
    result->analyzedType = targetType;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel