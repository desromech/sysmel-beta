#include "Environment/ASTProgramEntityExtensionNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTProgramEntityExtensionNode> ASTProgramEntityExtensionNodeTypeRegistration;

bool ASTProgramEntityExtensionNode::isASTProgramEntityExtensionNode() const
{
    return true;
}

AnyValuePtr ASTProgramEntityExtensionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitProgramEntityExtensionNode(selfFromThis());
}

SExpression ASTProgramEntityExtensionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"extension"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        programEntity->asSExpression(),
        body ? body->asSExpression() : nullptr,
    }};
}

void ASTProgramEntityExtensionNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(programEntity) aBlock(programEntity);
    if(body) aBlock(body);
}

} // End of namespace Environment
} // End of namespace Sysmel