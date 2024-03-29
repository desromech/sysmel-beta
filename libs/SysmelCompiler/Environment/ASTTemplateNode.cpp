#include "Environment/ASTTemplateNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        SExpressionIdentifier{{dllLinkageModeToString(dllLinkageMode)}},
        SExpressionIdentifier{{externalLanguageModeToString(externalLanguageMode)}},
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

} // End of namespace Environment
} // End of namespace Sysmel