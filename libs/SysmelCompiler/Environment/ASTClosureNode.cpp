#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTClosureNode> ASTClosureNodeTypeRegistration;

bool ASTClosureNode::isASTClosureNode() const
{
    return true;
}

AnyValuePtr ASTClosureNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitClosureNode(selfFromThis());
}

SExpression ASTClosureNode::asSExpression() const
{
    SExpressionList argumentsSExpression;
    argumentsSExpression.elements.reserve(arguments.size());
    for(const auto &arg : arguments )
        argumentsSExpression.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"closure"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        argumentsSExpression,
        resultType ? resultType->asSExpression() : nullptr,
        body->asSExpression(),
    }};
}

void ASTClosureNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    for(auto &arg : arguments)
        aBlock(arg);
    if(resultType) aBlock(resultType);
    if(body) aBlock(body);
}

} // End of namespace Environment
} // End of namespace Sysmel