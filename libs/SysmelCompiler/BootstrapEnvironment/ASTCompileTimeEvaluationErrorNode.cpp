#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/CompileTimeEvaluationError.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCompileTimeEvaluationErrorNode> ASTCompileTimeEvaluationErrorNodeTypeRegistration;

bool ASTCompileTimeEvaluationErrorNode::isASTCompileTimeEvaluationErrorNode() const
{
    return true;
}

AnyValuePtr ASTCompileTimeEvaluationErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCompileTimeEvaluationErrorNode(shared_from_this());
}

SExpression ASTCompileTimeEvaluationErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"compileTimeEvaluationError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTCompileTimeEvaluationErrorNode::asCompilationError()
{
    auto result = std::make_shared<CompileTimeEvaluationError> ();
    result->compileTimeEvaluationErrorNode = shared_from_this();
    result->caughtError = caughtError;
    return result;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius