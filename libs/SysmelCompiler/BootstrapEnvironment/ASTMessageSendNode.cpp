#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
        auto rawIdentifierValue = staticObjectCast<ASTLiteralValueNode> (selector)->value;
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

void ASTMessageSendNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(selector) aBlock(selector);
    if(receiver) aBlock(receiver);
    for(auto &arg : arguments)
        aBlock(arg);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius