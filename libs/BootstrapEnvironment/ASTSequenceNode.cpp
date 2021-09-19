#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSequenceNode> ASTSequenceNodeTypeRegistration;

bool ASTSequenceNode::isASTSequenceNode() const
{
    return true;
}

SExpression ASTSequenceNode::asSExpression() const
{
    SExpressionList sexprPragmas;
    sexprPragmas.elements.reserve(pragmas.size());
    for(const auto &pragma : pragmas)
        sexprPragmas.elements.push_back(pragma->asSExpression());

    SExpressionList sexpr;
    sexpr.elements.reserve(2 + expressions.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"sequence"}});
    sexpr.elements.push_back(sexprPragmas);
    for(const auto &expr : expressions)
        sexpr.elements.push_back(expr->asSExpression());

    return sexpr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius