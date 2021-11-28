#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius