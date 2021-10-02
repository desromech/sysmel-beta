#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
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
    return visitor->visitMessageSendNode(shared_from_this());
}

bool ASTMessageSendNode::isPureCompileTimeLiteralMessage() const
{
    if(receiver)
    {
        if(!receiver->isPureCompileTimeLiteralValueNode())
            return false;
    }
    else
    {
        if(!analyzedBoundMessage || !analyzedBoundMessageIsDirect)
            return false;
    }

    for(const auto &argument : arguments)
    {
        if(!argument->isPureCompileTimeLiteralValueNode())
            return false;
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
        selector->asSExpression(),
        receiver ? receiver->asSExpression() : nullptr,
        argumentsSExpression,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius