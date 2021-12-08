#include "Environment/ASTUpcastTypeConversionNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTUpcastTypeConversionNode> ASTUpcastTypeConversionNodeRegistration;

bool ASTUpcastTypeConversionNode::isASTUpcastTypeConversionNode() const
{
    return true;
}

bool ASTUpcastTypeConversionNode::isPureCompileTimeLiteralValueNode() const
{
    return expression->isPureCompileTimeLiteralValueNode();
}

bool ASTUpcastTypeConversionNode::isPureCompileTimeEvaluableNode() const
{
    return expression->isPureCompileTimeEvaluableNode();
}

AnyValuePtr ASTUpcastTypeConversionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitUpcastTypeConversionNode(selfFromThis());
}

SExpression ASTUpcastTypeConversionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"upcast"}},
        sourcePosition->asSExpression(),
        analyzedType->asSExpression(),
        expression ? expression->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel