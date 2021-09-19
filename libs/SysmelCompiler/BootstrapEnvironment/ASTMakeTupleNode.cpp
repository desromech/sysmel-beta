#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMakeTupleNode> ASTMakeTupleNodeTypeRegistration;

bool ASTMakeTupleNode::isASTMakeTupleNode() const
{
    return true;
}

AnyValuePtr ASTMakeTupleNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeTupleNode(shared_from_this());
}

SExpression ASTMakeTupleNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(2 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"tuple"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius