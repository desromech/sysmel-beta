#include "Environment/ASTFieldVariableAccessNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTFieldVariableAccessNode> ASTFieldVariableAccessNodeRegistration;

bool ASTFieldVariableAccessNode::isASTFieldVariableAccessNode() const
{
    return true;
}

AnyValuePtr ASTFieldVariableAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitFieldVariableAccessNode(selfFromThis());
}

SExpression ASTFieldVariableAccessNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"fieldVariableAccess"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        aggregate->asSExpression(),
        fieldVariable->asSExpression()
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel