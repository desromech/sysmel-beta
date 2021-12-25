#include "Environment/ASTQuasiQuotePatternExpansionNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuasiQuotePatternExpansionNode> ASTQuasiQuotePatternExpansionNodeTypeRegistration;

bool ASTQuasiQuotePatternExpansionNode::isASTQuasiQuotePatternExpansionNode() const
{
    return true;
}

AnyValuePtr ASTQuasiQuotePatternExpansionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuasiQuotePatternExpansionNode(selfFromThis());
}

SExpression ASTQuasiQuotePatternExpansionNode::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"quasiQuotePatternExpansion"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        pattern->asSExpression(),
        argumentsSExpr
    }};
}

void ASTQuasiQuotePatternExpansionNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(pattern) aBlock(pattern);
    for(auto &arg : arguments)
        aBlock(arg);
}

} // End of namespace Environment
} // End of namespace Sysmel