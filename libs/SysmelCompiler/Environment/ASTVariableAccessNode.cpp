#include "Environment/ASTVariableAccessNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Variable.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTVariableAccessNode> ASTVariableAccessNodeRegistration;

bool ASTVariableAccessNode::isASTVariableAccessNode() const
{
    return true;
}

AnyValuePtr ASTVariableAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitVariableAccessNode(selfFromThis());
}

SExpression ASTVariableAccessNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"variableAccess"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        variable->asSExpression()
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel