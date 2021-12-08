#include "Environment/ASTContinueNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel