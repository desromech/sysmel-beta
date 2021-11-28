#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTArgumentDefinitionNode> ASTArgumentDefinitionNodeTypeRegistration;

bool ASTArgumentDefinitionNode::isASTArgumentDefinitionNode() const
{
    return true;
}

AnyValuePtr ASTArgumentDefinitionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitArgumentDefinitionNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius