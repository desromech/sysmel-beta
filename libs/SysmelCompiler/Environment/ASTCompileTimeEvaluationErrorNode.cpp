#include "Environment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/CompileTimeEvaluationError.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTCompileTimeEvaluationErrorNode> ASTCompileTimeEvaluationErrorNodeTypeRegistration;

bool ASTCompileTimeEvaluationErrorNode::isASTCompileTimeEvaluationErrorNode() const
{
    return true;
}

AnyValuePtr ASTCompileTimeEvaluationErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCompileTimeEvaluationErrorNode(selfFromThis());
}

SExpression ASTCompileTimeEvaluationErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"compileTimeEvaluationError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTCompileTimeEvaluationErrorNode::asCompilationError()
{
    auto result = basicMakeObject<CompileTimeEvaluationError> ();
    result->compileTimeEvaluationErrorNode = selfFromThis();
    result->caughtError = caughtError;
    return result;
}
} // End of namespace Environment
} // End of namespace Sysmel