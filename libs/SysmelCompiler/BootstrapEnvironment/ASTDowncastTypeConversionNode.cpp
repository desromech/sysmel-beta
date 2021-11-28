#include "sysmel/BootstrapEnvironment/ASTDowncastTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius