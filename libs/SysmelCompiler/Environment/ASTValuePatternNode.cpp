#include "Environment/ASTValuePatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTFailPatternNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTValuePatternNode> ASTValuePatternNodeTypeRegistration;

bool ASTValuePatternNode::isASTValuePatternNode() const
{
    return true;
}

AnyValuePtr ASTValuePatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitValuePatternNode(selfFromThis());
}

SExpression ASTValuePatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"valuePattern"}},
        sourcePosition->asSExpression(),
        expectedValue->asSExpression()
    }};
}

ASTNodePtr ASTValuePatternNode::expandPatternNodeForExpectedTypeWith(const TypePtr &, const ASTNodePtr &patternValueNode, const ASTSemanticAnalyzerPtr &)
{
    auto builder = ASTBuilder::withSourcePosition(sourcePosition);
    auto isMatching = builder->sendToWithArguments(builder->literalSymbol("="), patternValueNode, {expectedValue});
    return builder->ifElse(isMatching, builder->failPattern());
}

} // End of namespace Environment
} // End of namespace Sysmel