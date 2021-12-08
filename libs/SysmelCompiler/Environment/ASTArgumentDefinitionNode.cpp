#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTArgumentDefinitionNode> ASTArgumentDefinitionNodeTypeRegistration;

bool ASTArgumentDefinitionNode::isASTArgumentDefinitionNode() const
{
    return true;
}

AnyValuePtr ASTArgumentDefinitionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitArgumentDefinitionNode(selfFromThis());
}

SExpression ASTArgumentDefinitionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"argument"}},
        sourcePosition->asSExpression(),
        identifier ? identifier->asSExpression() : nullptr,
        type ? type->asSExpression() : nullptr,
    }};
}

void ASTArgumentDefinitionNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(identifier) aBlock(identifier);
    if(type) aBlock(type);
}

} // End of namespace Environment
} // End of namespace Sysmel