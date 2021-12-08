#include "Environment/ASTMessageChainNode.hpp"
#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMessageChainNode> ASTMessageChainNodeTypeRegistration;

bool ASTMessageChainNode::isASTMessageChainNode() const
{
    return true;
}

AnyValuePtr ASTMessageChainNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMessageChainNode(selfFromThis());
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

} // End of namespace Environment
} // End of namespace Sysmel