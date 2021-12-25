#include "Environment/ASTQuasiQuoteExpander.hpp"
#include "Environment/ASTQuasiQuotePatternExpansionArgumentNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuasiQuoteExpander> ASTQuasiQuoteExpanderTypeRegistration;

ASTNodePtr ASTQuasiQuoteExpander::expandPatternWithArguments(const ASTNodePtr &pattern, const AnyValuePtrList &newExpansionArguments)
{
    auto expandedPattern = pattern;
    expansionArguments = newExpansionArguments;
    transformChildNode(expandedPattern);
    return expandedPattern;
}

AnyValuePtr ASTQuasiQuoteExpander::visitQuasiQuotePatternExpansionArgumentNode(const ASTQuasiQuotePatternExpansionArgumentNodePtr &node)
{
    sysmelAssert(!node->isSplice);
    auto result = expansionArguments[node->argumentIndex];
    sysmelAssert(result->isASTNode());
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel