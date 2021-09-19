#include "sysmel/BootstrapEnvironment/ASTMakeLiteralArrayNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMakeLiteralArrayNode> ASTMakeLiteralArrayNodeTypeRegistration;

bool ASTMakeLiteralArrayNode::isASTMakeLiteralArrayNode() const
{
    return true;
}

AnyValuePtr ASTMakeLiteralArrayNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeLiteralArrayNode(shared_from_this());
}

SExpression ASTMakeLiteralArrayNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(2 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"literalArray"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius