#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
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

AnyValuePtr ASTLexicalScopeNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLexicalScopeNode(shared_from_this());
}

SExpression ASTLexicalScopeNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"lexicalScope"}},
        sourcePosition->asSExpression(),
        body->asSExpression()
    }};
}

ASTNodePtr ASTLexicalScopeNode::asInlinedBlockBodyNode()
{
    return body ? body->asInlinedBlockBodyNode() : nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius