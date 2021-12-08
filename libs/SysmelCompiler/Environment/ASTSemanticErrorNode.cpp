#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/SemanticError.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSemanticErrorNode> ASTSemanticErrorNodeTypeRegistration;

bool ASTSemanticErrorNode::isASTSemanticErrorNode() const
{
    return true;
}

AnyValuePtr ASTSemanticErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSemanticErrorNode(selfFromThis());
}

SExpression ASTSemanticErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"semanticError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTSemanticErrorNode::asCompilationError()
{
    auto result = basicMakeObject<SemanticError> ();
    result->semanticErrorNode = selfFromThis();
    return result;
}
} // End of namespace Environment
} // End of namespace Sysmel