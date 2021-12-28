#include "Environment/ASTQuasiQuotePatternExpansionArgumentNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuasiQuotePatternExpansionArgumentNode> ASTQuasiQuotePatternExpansionArgumentNodeTypeRegistration;

bool ASTQuasiQuotePatternExpansionArgumentNode::isASTQuasiQuotePatternExpansionArgumentNode() const
{
    return true;
}

AnyValuePtr ASTQuasiQuotePatternExpansionArgumentNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuasiQuotePatternExpansionArgumentNode(selfFromThis());
}

SExpression ASTQuasiQuotePatternExpansionArgumentNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quasiQuotePatternExpansionArgument"}},
        sourcePosition->asSExpression(),
        isSplice,
        LargeInteger{argumentIndex}
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel