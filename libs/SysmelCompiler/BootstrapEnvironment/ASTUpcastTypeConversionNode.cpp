#include "sysmel/BootstrapEnvironment/ASTUpcastTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius