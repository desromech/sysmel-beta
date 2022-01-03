#include "Environment/ASTMakeAggregateNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMakeAggregateNode> ASTMakeAggregateNodeTypeRegistration;

bool ASTMakeAggregateNode::isASTMakeAggregateNode() const
{
    return true;
}

AnyValuePtr ASTMakeAggregateNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeAggregateNode(selfFromThis());
}

SExpression ASTMakeAggregateNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(4 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"aggregate"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    sexpr.elements.push_back(analyzedType ? analyzedType->asSExpression() : nullptr);
    sexpr.elements.push_back(aggregateType->asSExpression());
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

void ASTMakeAggregateNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    for(auto &el : elements)
        aBlock(el);
}

} // End of namespace Environment
} // End of namespace Sysmel