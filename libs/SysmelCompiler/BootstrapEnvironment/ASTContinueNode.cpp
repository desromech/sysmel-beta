#include "sysmel/BootstrapEnvironment/ASTContinueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTContinueNode> ASTContinueNodeTypeRegistration;

bool ASTContinueNode::isASTContinueNode() const
{
    return true;
}

AnyValuePtr ASTContinueNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitContinueNode(selfFromThis());
}

SExpression ASTContinueNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"continue"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius