#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMakeAssociationNode> ASTMakeAssociationNodeTypeRegistration;

bool ASTMakeAssociationNode::isASTMakeAssociationNode() const
{
    return true;
}

AnyValuePtr ASTMakeAssociationNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeAssociationNode(selfFromThis());
}

SExpression ASTMakeAssociationNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"association"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        key ? key->asSExpression() : nullptr,
        value ? value->asSExpression() : nullptr,
    }};
}

void ASTMakeAssociationNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(key) aBlock(key);
    if(value) aBlock(value);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius