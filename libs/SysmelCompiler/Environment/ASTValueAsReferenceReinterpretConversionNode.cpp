#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTValueAsReferenceReinterpretConversionNode> ASTValueAsReferenceReinterpretConversionNodeRegistration;

bool ASTValueAsReferenceReinterpretConversionNode::isASTValueAsReferenceReinterpretConversionNode() const
{
    return true;
}

AnyValuePtr ASTValueAsReferenceReinterpretConversionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitValueAsReferenceReinterpretConversionNode(selfFromThis());
}

SExpression ASTValueAsReferenceReinterpretConversionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"downcast"}},
        sourcePosition->asSExpression(),
        analyzedType->asSExpression(),
        expression ? expression->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel