#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTPragmaNode> ASTPragmaNodeTypeRegistration;

bool ASTPragmaNode::isASTPragmaNode() const
{
    return true;
}

AnyValuePtr ASTPragmaNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitPragmaNode(shared_from_this());
}

SExpression ASTPragmaNode::asSExpression() const
{
    SExpressionList argumentsSExpression;
    argumentsSExpression.elements.reserve(arguments.size());
    for(const auto &arg : arguments )
        argumentsSExpression.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"pragma"}},
        sourcePosition->asSExpression(),
        selector->asSExpression(),
        argumentsSExpression,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius