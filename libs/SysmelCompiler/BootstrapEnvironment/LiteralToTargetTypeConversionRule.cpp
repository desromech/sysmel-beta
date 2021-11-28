#include "sysmel/BootstrapEnvironment/LiteralToTargetTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/LiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralToTargetTypeConversionRule> valueAsVoidTypeConversionRuleRegistration;

TypeConversionRulePtr LiteralToTargetTypeConversionRule::uniqueInstance()
{
    static TypeConversionRulePtr singleton = basicMakeObject<LiteralToTargetTypeConversionRule> ();
    return singleton;
}

bool LiteralToTargetTypeConversionRule::canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const
{
    (void)sourceType;
    if(!node->isASTLiteralValueNode())    
        return false;

    auto literalValue = staticObjectCast<LiteralValue> (staticObjectCast<ASTLiteralValueNode> (node)->value);
    return literalValue->canBeConcretizedWithType(targetType);
}

size_t LiteralToTargetTypeConversionRule::getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const
{
    assert(node->isASTLiteralValueNode());

    auto literalValue = staticObjectCast<LiteralValue> (staticObjectCast<ASTLiteralValueNode> (node)->value);
    if(literalValue->getBestConcreteType() == targetType)
        return 5;
    else if(literalValue->getStandardConcreteType() == targetType)
        return 10;
    else
        return 15;
}

ASTNodePtr LiteralToTargetTypeConversionRule::convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const
{
    assert(node->isASTLiteralValueNode());

    auto literalValue = staticObjectCast<LiteralValue> (staticObjectCast<ASTLiteralValueNode> (node)->value);
    auto concreteValue = literalValue->concretizeWithType(targetType);
    if(!concreteValue)
        return semanticAnalyzer->recordSemanticErrorInNode(node, "Failed to concretize literal value.");

    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(concreteValue->asASTNodeRequiredInPosition(sourcePosition));
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius