#include "Environment/LiteralToTargetTypeConversionRule.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/LiteralValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralToTargetTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr LiteralToTargetTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeGlobalSingletonObject<LiteralToTargetTypeConversionRule> ();
    return singleton;
}

bool LiteralToTargetTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)sourceType;
    if(!node->isASTLiteralValueNode())    
        return false;

    auto literalValue = staticObjectCast<LiteralValue> (node.staticAs<ASTLiteralValueNode> ()->value);
    return literalValue->canBeConcretizedWithType(targetType);
}

TypeConversionCost LiteralToTargetTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    sysmelAssert(node->isASTLiteralValueNode());

    auto literalValue = staticObjectCast<LiteralValue> (node.staticAs<ASTLiteralValueNode> ()->value);
    if(literalValue->getBestConcreteType() == targetType)
        return TypeConversionCost(DirectTypeConversionCost::LiteralBestTarget);
    else if(literalValue->getStandardConcreteType() == targetType)
        return TypeConversionCost(DirectTypeConversionCost::LiteralStandardTarget);
    else
        return TypeConversionCost(DirectTypeConversionCost::LiteralTarget);
}

ASTNodePtr LiteralToTargetTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    sysmelAssert(node->isASTLiteralValueNode());

    auto literalValue = staticObjectCast<LiteralValue> (node.staticAs<ASTLiteralValueNode> ()->value);
    auto concreteValue = literalValue->concretizeWithType(targetType);
    if(!concreteValue)
        return semanticAnalyzer->recordSemanticErrorInNode(node, "Failed to concretize literal value.");

    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(concreteValue->asASTNodeRequiredInPosition(sourcePosition));
}

} // End of namespace Environment
} // End of namespace Sysmel