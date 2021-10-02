#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
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

bool ASTLiteralValueNode::isASTLiteralSymbolValue() const
{
    return value && value->isLiteralSymbol();
}

AnyValuePtr ASTLiteralValueNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLiteralValueNode(shared_from_this());
}

void ASTLiteralValueNode::setValueAndType(const AnyValuePtr &theValue)
{
    value = theValue;
    type = theValue->getType();
}

bool ASTLiteralValueNode::isPureCompileTimeLiteralValueNode() const
{
    return value && value->isPureCompileTimeLiteralValue();
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