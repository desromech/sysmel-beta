#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMessageChainNode> ASTMessageChainNodeTypeRegistration;

bool ASTMessageChainNode::isASTMessageChainNode() const
{
    return true;
}

AnyValuePtr ASTMessageChainNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMessageChainNode(shared_from_this());
}

SExpression ASTMessageChainNode::asSExpression() const
{
    SExpressionList messagesSExpression;
    messagesSExpression.elements.reserve(messages.size());
    for(const auto &message : messages )
        messagesSExpression.elements.push_back(message->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"chain"}},
        sourcePosition->asSExpression(),
        receiver ? receiver->asSExpression() : nullptr,
        messagesSExpression,
    }};
}

void ASTMessageChainNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(receiver) aBlock(receiver);
    for(auto &message : messages)
        aBlock(message);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius