#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/SemanticError.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSemanticErrorNode> ASTSemanticErrorNodeTypeRegistration;

bool ASTSemanticErrorNode::isASTSemanticErrorNode() const
{
    return true;
}

AnyValuePtr ASTSemanticErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSemanticErrorNode(shared_from_this());
}

SExpression ASTSemanticErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"semanticError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTSemanticErrorNode::asCompilationError()
{
    auto result = std::make_shared<SemanticError> ();
    result->semanticErrorNode = shared_from_this();
    return result;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius