#include "Environment/ASTMakeLiteralArrayNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMakeLiteralArrayNode> ASTMakeLiteralArrayNodeTypeRegistration;

bool ASTMakeLiteralArrayNode::isASTMakeLiteralArrayNode() const
{
    return true;
}

AnyValuePtr ASTMakeLiteralArrayNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeLiteralArrayNode(selfFromThis());
}

SExpression ASTMakeLiteralArrayNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(2 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"literalArray"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    sexpr.elements.push_back(analyzedType ? analyzedType->asSExpression() : nullptr);
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

void ASTMakeLiteralArrayNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    for(auto &el : elements)
        aBlock(el);
}

} // End of namespace Environment
} // End of namespace Sysmel