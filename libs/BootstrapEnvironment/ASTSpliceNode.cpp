#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSpliceNode> ASTSpliceNodeTypeRegistration;

bool ASTSpliceNode::isASTSpliceNode() const
{
    return true;
}

SExpression ASTSpliceNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"splice"}},
        sourcePosition->asSExpression(),
        expression->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius