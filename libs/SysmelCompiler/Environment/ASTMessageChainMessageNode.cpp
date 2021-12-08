#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMessageChainMessageNode> ASTMessageChainMessageNodeTypeRegistration;

bool ASTMessageChainMessageNode::isASTMessageChainMessageNode() const
{
    return true;
}

AnyValuePtr ASTMessageChainMessageNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMessageChainMessageNode(selfFromThis());
}

SExpression ASTMessageChainMessageNode::asSExpression() const
{
    SExpressionList argumentsSExpression;
    argumentsSExpression.elements.reserve(arguments.size());
    for(const auto &arg : arguments )
        argumentsSExpression.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"message"}},
        sourcePosition->asSExpression(),
        selector->asSExpression(),
        argumentsSExpression,
    }};
}

ASTMessageSendNodePtr ASTMessageChainMessageNode::asMessageSendNodeWithReceiver(const ASTNodePtr &newReceiver)
{
    auto result = basicMakeObject<ASTMessageSendNode> ();
    result->sourcePosition = sourcePosition;
    result->receiver = newReceiver;
    result->selector = selector;
    result->arguments = arguments;
    return result;
}

void ASTMessageChainMessageNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(selector) aBlock(selector);
    for(auto &arg : arguments)
        aBlock(arg);
}

} // End of namespace Environment
} // End of namespace Sysmel