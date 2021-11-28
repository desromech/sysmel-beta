#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTClosureNode> ASTClosureNodeTypeRegistration;

bool ASTClosureNode::isASTClosureNode() const
{
    return true;
}

AnyValuePtr ASTClosureNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitClosureNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius