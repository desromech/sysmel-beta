#include "Environment/ASTValueAsVoidTypeConversionNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTValueAsVoidTypeConversionNode> ASTValueAsVoidTypeConversionNodeRegistration;

bool ASTValueAsVoidTypeConversionNode::isASTValueAsVoidTypeConversionNode() const
{
    return true;
}

AnyValuePtr ASTValueAsVoidTypeConversionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitValueAsVoidTypeConversionNode(selfFromThis());
}

SExpression ASTValueAsVoidTypeConversionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"valueAsVoid"}},
        sourcePosition->asSExpression(),
        analyzedType->asSExpression(),
        expression ? expression->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel