#include "Environment/ASTBindingPatternNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTNeverPatternNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTBindingPatternNode> ASTBindingPatternNodeTypeRegistration;

bool ASTBindingPatternNode::isASTBindingPatternNode() const
{
    return true;
}

AnyValuePtr ASTBindingPatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitBindingPatternNode(selfFromThis());
}

SExpression ASTBindingPatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"bindingPattern"}},
        sourcePosition->asSExpression(),

        identifier ? identifier->asSExpression() : nullptr,
        expectedType ? expectedType->asSExpression() : nullptr,
        expectedValue ? expectedValue->asSExpression() : nullptr
    }};
}

bool ASTBindingPatternNode::isAlwaysMatchingPattern() const
{
    return !expectedValue || expectedValue->isAlwaysMatchingPattern();
}

bool ASTBindingPatternNode::isNeverMatchingPattern() const
{
    return expectedValue && expectedValue->isNeverMatchingPattern();
}

ASTNodePtr ASTBindingPatternNode::optimizePatternNodeForExpectedTypeWith(const TypePtr &type, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto result = shallowCloneObject(selfFromThis());

    auto bindingType = type;

    // Evaluate and check the expected type.
    if(result->expectedType)
    {
        result->expectedType = semanticAnalyzer->evaluateTypeExpression(result->expectedType);
        if(result->expectedType->isASTErrorNode())
            return result->expectedType;

        bindingType = unwrapTypeFromLiteralValue(result->expectedType);
        if(!type->matchesValueTypeInPattern(bindingType))
            return asNeverPatternNode();
    }

    // Check  the expected value, if present.
    if(result->expectedValue)
    {
        result->expectedValue = result->expectedValue->optimizePatternNodeForExpectedTypeWith(bindingType, semanticAnalyzer);
        if(result->expectedValue->isNeverMatchingPattern())
            return asNeverPatternNode();

        if(!expectedType)
            bindingType = result->expectedValue->analyzedType->asDecayedType();
    }

    result->analyzedType = bindingType;
    return result;
}

ASTNodePtr ASTBindingPatternNode::expandPatternNodeForExpectedTypeWith(const TypePtr &type, const ASTNodePtr &patternValueNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!expectedType)
    {
        auto expandedValue = patternValueNode;
        if(expectedValue)
            expandedValue = expectedValue->expandPatternNodeForExpectedTypeWith(type, patternValueNode, semanticAnalyzer);

        if(!identifier)
            return expandedValue;
        
        auto localVariableNode = basicMakeObject<ASTLocalVariableNode> ();
        localVariableNode->name = identifier;
        localVariableNode->sourcePosition = sourcePosition;
        localVariableNode->typeInferenceMode = TypeInferenceMode::TemporaryReference;
        localVariableNode->initialValue = expandedValue;
        return localVariableNode;
    }

    sysmelAssert("TODO: Implement this case" && false);
}

} // End of namespace Environment
} // End of namespace Sysmel