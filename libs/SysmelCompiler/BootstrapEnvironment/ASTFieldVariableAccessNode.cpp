#include "sysmel/BootstrapEnvironment/ASTFieldVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/FieldVariable.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius