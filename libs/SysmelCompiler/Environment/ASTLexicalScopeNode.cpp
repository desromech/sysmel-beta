#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

ASTPragmaNodePtr ASTLexicalScopeNode::getPragmaNamed(const AnyValuePtr &requestedPragmaSelector)
{
    return body ? body->getPragmaNamed(requestedPragmaSelector) : nullptr;
}

} // End of namespace Environment
} // End of namespace Sysmel