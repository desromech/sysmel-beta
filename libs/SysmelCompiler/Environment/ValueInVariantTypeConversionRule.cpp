#include "Environment/ValueInVariantTypeConversionRule.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/Type.hpp"
#include "Environment/VariantType.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTMakeVariantNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include <iostream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ValueInVariantTypeConversionRule> identityTypeConversionRuleRegistration;

TypeConversionRulePtr ValueInVariantTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<ValueInVariantTypeConversionRule> ();
    return singleton;
}

bool ValueInVariantTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    auto decayedSourceType = sourceType->asDecayedType();
    auto decayedTargetType = targetType->asDecayedType();
    if(!decayedTargetType->isVariantType())
        return false;
    
    return decayedTargetType.staticAs<VariantType> ()->includesType(decayedSourceType);
}

TypeConversionCost ValueInVariantTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    (void)sourceType;
    (void)targetType;
    return TypeConversionCost(DirectTypeConversionCost::ValueInVariant);
}

ASTNodePtr ValueInVariantTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    sysmelAssert(node->analyzedType);

    auto valueInVariant = basicMakeObject<ASTMakeVariantNode> ();
    valueInVariant->sourcePosition = sourcePosition;
    valueInVariant->variantType = targetType->asDecayedType()->asASTNodeRequiredInPosition(sourcePosition);
    valueInVariant->value = node;

    // Leave the remaining of the conversion to the analyzer.
    return semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(valueInVariant, targetType);
}

} // End of namespace Environment
} // End of namespace Sysmel