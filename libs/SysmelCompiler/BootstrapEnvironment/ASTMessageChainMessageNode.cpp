#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMessageChainMessageNode> ASTMessageChainMessageNodeTypeRegistration;

bool ASTMessageChainMessageNode::isASTMessageChainMessageNode() const
{
    return true;
}

AnyValuePtr ASTMessageChainMessageNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMessageChainMessageNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius