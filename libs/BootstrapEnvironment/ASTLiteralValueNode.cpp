#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTLiteralValueNode> ASTLiteralValueNodeTypeRegistration;

bool ASTLiteralValueNode::isASTLiteralValueNode() const
{
    return true;
}

void ASTLiteralValueNode::setValueAndType(const AnyValuePtr &theValue)
{
    value = theValue;
    type = theValue->getType();
}

SExpression ASTLiteralValueNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"literal"}},
        sourcePosition->asSExpression(),
        value ? value->asSExpression() : nullptr,
        type ? (type->isASTNode() ? type->asSExpression() : SExpressionSymbol{type->asString()}) : nullptr
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius