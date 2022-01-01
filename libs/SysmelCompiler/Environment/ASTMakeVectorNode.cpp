#include "Environment/ASTMakeVectorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMakeVectorNode> ASTMakeVectorNodeTypeRegistration;

bool ASTMakeVectorNode::isASTMakeVectorNode() const
{
    return true;
}

AnyValuePtr ASTMakeVectorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeVectorNode(selfFromThis());
}

SExpression ASTMakeVectorNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(3 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"vector"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    sexpr.elements.push_back(analyzedType ? analyzedType->asSExpression() : nullptr);
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

void ASTMakeVectorNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    for(auto &el : elements)
        aBlock(el);
}

} // End of namespace Environment
} // End of namespace Sysmel