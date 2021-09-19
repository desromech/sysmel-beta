#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTLexicalScopeNode> ASTLexicalScopeNodeTypeRegistration;

bool ASTLexicalScopeNode::isASTLexicalScopeNode() const
{
    return true;
}

SExpression ASTLexicalScopeNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"lexicalScope"}},
        sourcePosition->asSExpression(),
        body->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius