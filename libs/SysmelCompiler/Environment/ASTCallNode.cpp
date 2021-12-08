#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTCallNode> ASTCallNodeTypeRegistration;

bool ASTCallNode::isASTCallNode() const
{
    return true;
}

AnyValuePtr ASTCallNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCallNode(selfFromThis());
}

SExpression ASTCallNode::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"call"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        function->asSExpression(),
        argumentsSExpr
    }};
}

void ASTCallNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(function) aBlock(function);
    for(auto &arg : arguments)
        aBlock(arg);
}

} // End of namespace Environment
} // End of namespace Sysmel