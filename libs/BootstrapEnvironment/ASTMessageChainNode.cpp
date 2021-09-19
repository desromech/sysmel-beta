#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius