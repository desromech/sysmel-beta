#include "Environment/ASTVectorSwizzleNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTVectorSwizzleNode> ASTVectorSwizzleNodeTypeRegistration;

bool ASTVectorSwizzleNode::isASTVectorSwizzleNode() const
{
    return true;
}

AnyValuePtr ASTVectorSwizzleNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitVectorSwizzleNode(selfFromThis());
}

SExpression ASTVectorSwizzleNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(4 + selectedElements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"vectorSwizzle"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    sexpr.elements.push_back(analyzedType ? analyzedType->asSExpression() : nullptr);
    sexpr.elements.push_back(vector->asSExpression());
    for(auto element : selectedElements)
        sexpr.elements.push_back(LargeInteger{element});

    return sexpr;
}

void ASTVectorSwizzleNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    aBlock(vector);
}

} // End of namespace Environment
} // End of namespace Sysmel