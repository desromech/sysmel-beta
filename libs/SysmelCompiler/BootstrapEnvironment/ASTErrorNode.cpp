#include "sysmel/BootstrapEnvironment/ASTErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/CompilationError.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTErrorNode> ASTErrorNodeTypeRegistration;

bool ASTErrorNode::isASTErrorNode() const
{
    return true;
}

AnyValuePtr ASTErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitErrorNode(shared_from_this());
}

SExpression ASTErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"error"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTErrorNode::asCompilationError()
{
    auto result = std::make_shared<CompilationError> ();
    result->setMessageText(errorMessage);
    return result;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius