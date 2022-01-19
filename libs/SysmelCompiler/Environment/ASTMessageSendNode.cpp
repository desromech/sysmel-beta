#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"

#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTAnyValuePatternNode.hpp"
#include "Environment/ASTAlternativesPatternNode.hpp"
#include "Environment/ASTBindingPatternNode.hpp"
#include "Environment/ASTRangePatternNode.hpp"
#include "Environment/ASTPredicatedPatternNode.hpp"

#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMessageSendNode> ASTMessageSendNodeTypeRegistration;

bool ASTMessageSendNode::isASTMessageSendNode() const
{
    return true;
}

AnyValuePtr ASTMessageSendNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMessageSendNode(selfFromThis());
}

bool ASTMessageSendNode::isPureCompileTimeLiteralMessage() const
{
    if(receiver)
    {
        if(isPureMessageSend)
        {
            if(!receiver->isPureCompileTimeEvaluableNode())
                return false;
        }
        else
        {
            if(!receiver->isPureCompileTimeLiteralValueNode())
                return false;
        }
    }
    else
    {
        if(!analyzedBoundMessage || !analyzedBoundMessageIsDirect)
            return false;
    }

    for(const auto &argument : arguments)
    {
        if(isPureMessageSend)
        {
            if(!argument->isPureCompileTimeEvaluableNode())
                return false;
        }
        else
        {
            if(!argument->isPureCompileTimeLiteralValueNode())
                return false;
        }
    }

    return true;
}

SExpression ASTMessageSendNode::asSExpression() const
{
    SExpressionList argumentsSExpression;
    argumentsSExpression.elements.reserve(arguments.size());
    for(const auto &arg : arguments )
        argumentsSExpression.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"send"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        selector->asSExpression(),
        receiver ? receiver->asSExpression() : nullptr,
        argumentsSExpression,
    }};
}

ASTNodePtr ASTMessageSendNode::parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!receiver && arguments.size() == 1 && selector->isASTLiteralSymbolValue())
    {
        auto rawIdentifierValue = selector.staticAs<ASTLiteralValueNode> ()->value;
        auto identifier = rawIdentifierValue->asUnarySelectorConvertedToIdentifier();
        if(identifier && !identifier->isUndefined())
        {
            auto identifierNode = basicMakeObject<ASTLiteralValueNode> ();
            identifierNode->sourcePosition = selector->sourcePosition;
            identifierNode->setValueAndType(identifier);

            auto result = basicMakeObject<ASTArgumentDefinitionNode> ();
            result->sourcePosition = sourcePosition;
            result->identifier = identifierNode;
            result->type = arguments[0];
            return result;
        }
    }

    return SuperType::parseAsArgumentNodeWith(semanticAnalyzer);
}

ASTNodePtr ASTMessageSendNode::parseAsPatternNode()
{
    if(selector->isASTLiteralSymbolValue())
    {
        auto selectorValue = selector.staticAs<ASTLiteralValueNode> ()->value;

        if(arguments.empty())
        {
            if(receiver && receiver->isASTIdentifierReferenceNode())
            {
                auto identifier = receiver.staticAs<ASTIdentifierReferenceNode> ();
                if(validAnyValue(identifier->identifier)->asString() == "let")
                {
                    auto binding = basicMakeObject<ASTBindingPatternNode> ();
                    binding->sourcePosition = sourcePosition;
                    binding->identifier = selectorValue;
                    return binding;
                }
            }
        }
        else if(arguments.size() == 1)
        {
            auto selectorString = selectorValue->asString();
            if(receiver)
            {
                if(selectorString == ":=" || selectorString == "type:")
                {
                    // Speculatively attempt to parse as a binding pattern node.
                    auto receiverBindingPattern = receiver->parseAsBindingPatternNode();
                    if(!receiverBindingPattern->isASTBindingPatternNode())
                        return SuperType::parseAsPatternNode();

                    auto bindingPatternNode = shallowCloneObject(staticObjectCast<ASTBindingPatternNode> (receiverBindingPattern));
                    bindingPatternNode->sourcePosition = sourcePosition;
                    if(selectorString == "type:")
                        bindingPatternNode->expectedType = arguments[0]->parseAsPatternNode();
                    else // if(selectorString == ":=")
                        bindingPatternNode->expectedValue = arguments[0]->parseAsPatternNode();
                    return bindingPatternNode;
                }
                else if(selectorString == "to:" || selectorString == "until:")
                {
                    auto range = basicMakeObject<ASTRangePatternNode> ();
                    range->sourcePosition = sourcePosition;
                    range->startNode = receiver->parseAsPatternNode();
                    range->endNode = arguments[0]->parseAsPatternNode();
                    range->isInclusive = selectorString == "to:";
                    return range;
                }
                else if(selectorString == "|")
                {
                    auto previousAlternative = receiver->parseAsPatternNode();
                    auto nextAlternative = arguments[0]->parseAsPatternNode();

                    auto alternatives = basicMakeObject<ASTAlternativesPatternNode> ();
                    alternatives->sourcePosition = sourcePosition;

                    // Add the previous alternatives.
                    if(previousAlternative->isASTAlternativesPatternNode())
                        alternatives->alternatives = previousAlternative.staticAs<ASTAlternativesPatternNode> ()->alternatives;
                    else
                        alternatives->alternatives.push_back(previousAlternative);

                    // Add next alternatives
                    if(nextAlternative->isASTAlternativesPatternNode())
                    {
                        auto &nextAlternatives = nextAlternative.staticAs<ASTAlternativesPatternNode> ()->alternatives;
                        alternatives->alternatives.insert(alternatives->alternatives.end(), nextAlternatives.begin(), nextAlternatives.end());
                    }
                    else
                    {
                        alternatives->alternatives.push_back(nextAlternative);
                    }

                    return alternatives;
                }
            }

            // Predicated pattern
            if(selectorString == "when:")
            {
                ASTNodePtr receiverPattern;
                if(receiver)
                {
                    receiverPattern = receiver->parseAsPatternNode();
                }
                else
                {
                    auto anyPattern = basicMakeObject<ASTAnyValuePatternNode> ();
                    anyPattern->sourcePosition = sourcePosition;
                    receiverPattern = anyPattern;
                }

                auto predicatedPattern = basicMakeObject<ASTPredicatedPatternNode> ();
                predicatedPattern->sourcePosition = sourcePosition;
                predicatedPattern->expectedPattern = receiverPattern;
                predicatedPattern->predicate = arguments[0];
                return predicatedPattern;
            }
        }
    }

    return SuperType::parseAsPatternNode();
}

void ASTMessageSendNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(selector) aBlock(selector);
    if(receiver) aBlock(receiver);
    for(auto &arg : arguments)
        aBlock(arg);
}

} // End of namespace Environment
} // End of namespace Sysmel