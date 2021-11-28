#include "sysmel/BootstrapEnvironment/ASTTemplateNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTTemplateNode> ASTTemplateNodeTypeRegistration;

bool ASTTemplateNode::isASTTemplateNode() const
{
    return true;
}

AnyValuePtr ASTTemplateNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitTemplateNode(selfFromThis());
}

SExpression ASTTemplateNode::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(const auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"template"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        name ? name->asSExpression() : nullptr,
        argumentsSExpr,
        body ? body->asSExpression() : nullptr,
    }};
}

void ASTTemplateNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(name) aBlock(name);
    for(auto &arg : arguments)
        aBlock(arg);
    if(body) aBlock(body);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius