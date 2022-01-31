#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/LiteralValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

bool ASTLiteralValueNode::isASTIdentifierSymbolValue() const
{
    return value && value->isLiteralIdentifierSymbol();;
}

bool ASTLiteralValueNode::isASTLiteralTypeNode() const
{
    return value && value->isType();
}

AnyValuePtr ASTLiteralValueNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLiteralValueNode(selfFromThis());
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

bool ASTLiteralValueNode::isPureCompileTimeEvaluableNode() const
{
    return true;
}

SExpression ASTLiteralValueNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"literal"}},
        value ? value->asSExpression() : nullptr,
        type ? (type->isASTNode() ? type->asSExpression() : SExpressionSymbol{type->asString()}) : nullptr,
        sourcePosition->asSExpression(),
    }};
}

bool ASTLiteralValueNode::isValidForCachingTypeConversionRules() const
{
    return !type->isSubtypeOf(LiteralValue::__staticType__());
}

} // End of namespace Environment
} // End of namespace Sysmel