#include "Environment/ASTDowncastTypeConversionNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTDowncastTypeConversionNode> ASTDowncastTypeConversionNodeRegistration;

bool ASTDowncastTypeConversionNode::isASTDowncastTypeConversionNode() const
{
    return true;
}

AnyValuePtr ASTDowncastTypeConversionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitDowncastTypeConversionNode(selfFromThis());
}

SExpression ASTDowncastTypeConversionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"downcast"}},
        sourcePosition->asSExpression(),
        analyzedType->asSExpression(),
        expression ? expression->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel