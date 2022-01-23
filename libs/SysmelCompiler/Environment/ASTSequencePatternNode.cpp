#include "Environment/ASTSequencePatternNode.hpp"
#include "Environment/ASTNeverPatternNode.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTFailPatternNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTIfNode.hpp"

#include "Environment/Type.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Wrappers.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSequencePatternNode> ASTSequencePatternNodeTypeRegistration;

bool ASTSequencePatternNode::isASTSequencePatternNode() const
{
    return true;
}

AnyValuePtr ASTSequencePatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSequencePatternNode(selfFromThis());
}

SExpression ASTSequencePatternNode::asSExpression() const
{
    SExpressionList result;
    result.elements.reserve(3);
    result.elements.push_back(SExpressionIdentifier{{"sequencePattern"}});
    result.elements.push_back(sourcePosition->asSExpression());
    result.elements.push_back(expectedSequenceType ? expectedSequenceType->asSExpression() : nullptr);
    
    for(auto &el : elements)
        result.elements.push_back(el->asSExpression());

    return result;
}

ASTNodePtr ASTSequencePatternNode::optimizePatternNodeForExpectedTypeWith(const TypePtr &type, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto result = shallowCloneObject(selfFromThis());

    auto sequenceType = type;

    if(result->expectedSequenceType)
    {
        result->expectedSequenceType = semanticAnalyzer->evaluateTypeExpression(result->expectedSequenceType);
        if(result->expectedSequenceType->isASTErrorNode())
            return result->expectedSequenceType;

        sequenceType = unwrapTypeFromLiteralValue(result->expectedSequenceType);
        if(!sequenceType->matchesValueTypeInPattern(type->asDecayedType()))
            return asNeverPatternNode();
    }
    else
    {
        result->expectedSequenceType = type->asASTNodeRequiredInPosition(sourcePosition);
    }

    bool isSequenceablePatternType = sequenceType->perform<bool> ("isSequencePatternType");
    if(!isSequenceablePatternType)
        return asNeverPatternNode();

    auto minSize = sequenceType->perform<uint64_t> ("sequencePatternTypeMinSize");
    auto maxSize = sequenceType->perform<uint64_t> ("sequencePatternTypeMaxSize");
    auto patternSize = elements.size();

    if(!(minSize <= patternSize && patternSize <= maxSize))
        return asNeverPatternNode();

    result->elementTypes.reserve(elements.size());

    for(size_t i = 0; i < elements.size(); ++i)
    {
        auto elementType = sequenceType->perform<TypePtr> ("sequencePatternTypeAt:", uint32_t(i));
        if(!elementType)
            return asNeverPatternNode();
        
        auto element = elements[i]->optimizePatternNodeForExpectedTypeWith(elementType, semanticAnalyzer);
        if(element->isNeverMatchingPattern())
            return asNeverPatternNode();

        result->elements[i] = element;
        result->elementTypes.push_back(elementType);
    }
    result->analyzedType = sequenceType;
    return result;
}

ASTNodePtr ASTSequencePatternNode::expandPatternNodeForExpectedTypeWith(const TypePtr &valueType, const ASTNodePtr &patternValueNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto sequenceNode = basicMakeObject<ASTSequenceNode> ();
    sequenceNode->sourcePosition = sourcePosition;

    auto [binding, identifier] = expandHiddenBindingWithExpandedType(patternValueNode, analyzedType, valueType, semanticAnalyzer);
    sequenceNode->expressions.push_back(binding);

    // Check the size.
    auto builder = ASTBuilder::withSourcePosition(sourcePosition);

    {
        auto sequenceSize = builder->sendToWithArguments(builder->literalSymbol("__sequencePatternSize__"), identifier, {});
        auto expectedSize = builder->literal(wrapValue(uint64_t(elements.size())));
        auto hasNotExpectedSize = builder->sendToWithArguments(builder->literalSymbol("~="), sequenceSize, {expectedSize});

        auto sizeCheck = builder->ifThen(hasNotExpectedSize, builder->failPattern());
        sequenceNode->expressions.push_back(sizeCheck);
    }

    // Expand the different elements.
    auto sequenceAtSelector =  builder->literalSymbol("__sequencePatternAt__:");
    for(size_t i = 0; i < elements.size(); ++i)
    {
        auto &elementPattern = elements[i];
        auto &elementType = elementTypes[i];
        if(elementPattern->isASTAnyValuePatternNode())
            continue;

        auto index = builder->literal(wrapValue(uint64_t(i)));
        auto elementMessage = builder->sendToWithArguments(sequenceAtSelector, identifier, {index});

        auto expandedElementPattern = elementPattern->expandPatternNodeForExpectedTypeWith(elementType, elementMessage, semanticAnalyzer);
        sequenceNode->expressions.push_back(expandedElementPattern);
    }

    return sequenceNode;
}

bool ASTSequencePatternNode::isAlwaysMatchingPattern() const
{
    for(auto & el : elements)
    {
        if(!el->isAlwaysMatchingPattern())
            return false;
    }
    return true;
}

bool ASTSequencePatternNode::isNeverMatchingPattern() const
{
    for(auto & el : elements)
    {
        if(el->isNeverMatchingPattern())
            return true;
    }
    return false;
}


} // End of namespace Environment
} // End of namespace Sysmel