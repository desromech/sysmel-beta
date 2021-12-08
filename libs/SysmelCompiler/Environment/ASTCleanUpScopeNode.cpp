#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTCleanUpScopeNode> ASTCleanUpScopeNodeRegistration;

bool ASTCleanUpScopeNode::isASTCleanUpScopeNode() const
{
    return true;
}

AnyValuePtr ASTCleanUpScopeNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCleanUpScopeNode(selfFromThis());
}

SExpression ASTCleanUpScopeNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"cleanUpScope"}},
        analyzedType ? analyzedType->asSExpression() : nullptr,
        sourcePosition->asSExpression(),
        body->asSExpression()
    }};
}

ASTNodePtr ASTCleanUpScopeNode::asInlinedBlockBodyNode()
{
    return body ? body->asInlinedBlockBodyNode() : nullptr;
}

void ASTCleanUpScopeNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(body) aBlock(body);
}

} // End of namespace Environment
} // End of namespace Sysmel