#include "Environment/ASTErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTErrorNode> ASTErrorNodeTypeRegistration;

bool ASTErrorNode::isASTErrorNode() const
{
    return true;
}

AnyValuePtr ASTErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitErrorNode(selfFromThis());
}

SExpression ASTErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"error"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTErrorNode::asCompilationError()
{
    auto result = basicMakeObject<CompilationError> ();
    result->setMessageText(errorMessage);
    return result;
}

ASTNodePtr ASTErrorNode::parseAsPatternNode()
{
    return selfFromThis();
}

} // End of namespace Environment
} // End of namespace Sysmel