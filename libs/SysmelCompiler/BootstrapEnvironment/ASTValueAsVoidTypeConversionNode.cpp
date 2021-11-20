#include "sysmel/BootstrapEnvironment/ASTValueAsVoidTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTValueAsVoidTypeConversionNode> ASTValueAsVoidTypeConversionNodeRegistration;

bool ASTValueAsVoidTypeConversionNode::isASTValueAsVoidTypeConversionNode() const
{
    return true;
}

AnyValuePtr ASTValueAsVoidTypeConversionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitValueAsVoidTypeConversionNode(shared_from_this());
}

SExpression ASTValueAsVoidTypeConversionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"valueAsVoid"}},
        sourcePosition->asSExpression(),
        analyzedType->asSExpression(),
        expression ? expression->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius