#include "Environment/ValueAsConstRefTypeConversionRule.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include <iostream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ValueAsConstRefTypeConversionRule> identityTypeConversionRuleRegistration;

TypeConversionRulePtr ValueAsConstRefTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<ValueAsConstRefTypeConversionRule> ();
    return singleton;
}

bool ValueAsConstRefTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    auto undecoratedSourceType = sourceType->asUndecoratedType();
    if(undecoratedSourceType->isReferenceLikeType())
        return false;
    
    if(!targetType->isReferenceType())
        return false;
    
    auto targetBaseType = targetType->getBaseType();
    if(!targetBaseType->isConstDecoratedType())
        return false;
    
    return undecoratedSourceType->isSubtypeOf(targetBaseType->asUndecoratedType());
}

TypeConversionCost ValueAsConstRefTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)node;
    (void)sourceType;
    (void)targetType;
    return TypeConversionCost(DirectTypeConversionCost::ValueAsConstRef);
}

ASTNodePtr ValueAsConstRefTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    (void)semanticAnalyzer;
    sysmelAssert(node->analyzedType);

    auto undecoratedSourceType = node->analyzedType->asUndecoratedType();
    ASTNodePtr valueToConstRef;
    if(undecoratedSourceType->isPassedByReference())
    {
        auto result = basicMakeObject<ASTValueAsReferenceReinterpretConversionNode> ();
        result->sourcePosition = sourcePosition;
        result->expression = node;
        result->analyzedType = undecoratedSourceType->withConst()->ref();
        valueToConstRef = result;
    }
    else
    {
        auto temp = basicMakeObject<ASTLocalVariableNode> ();
        temp->sourcePosition = sourcePosition;
        temp->initialValue = node;
        temp->isMutable = true;
        valueToConstRef = temp;
    }

    // Leave the upcast conversion to the analyzer.
    return semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(valueToConstRef, targetType);
}

} // End of namespace Environment
} // End of namespace Sysmel