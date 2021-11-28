#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
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
    return visitor->visitLexicalScopeNode(selfFromThis());
}

SExpression ASTLexicalScopeNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"lexicalScope"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        body->asSExpression()
    }};
}

ASTNodePtr ASTLexicalScopeNode::asInlinedBlockBodyNode()
{
    return body ? body->asInlinedBlockBodyNode() : nullptr;
}

void ASTLexicalScopeNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(body) aBlock(body);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius