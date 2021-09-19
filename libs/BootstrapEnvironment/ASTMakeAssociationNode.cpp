#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMakeAssociationNode> ASTMakeAssociationNodeTypeRegistration;

bool ASTMakeAssociationNode::isASTMakeAssociationNode() const
{
    return true;
}

SExpression ASTMakeAssociationNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"association"}},
        sourcePosition->asSExpression(),
        key ? key->asSExpression() : nullptr,
        value ? value->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius