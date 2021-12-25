#include "Environment/ASTQuasiQuoteAnalyzer.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTQuasiQuotePatternExpansionNode.hpp"
#include "Environment/ASTQuasiQuotePatternExpansionArgumentNode.hpp"
#include "Environment/ASTQuasiQuoteNode.hpp"
#include "Environment/ASTQuasiUnquoteNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuasiQuoteAnalyzer> ASTQuasiQuoteAnalyzerTypeRegistration;

ASTQuasiQuotePatternExpansionNodePtr ASTQuasiQuoteAnalyzer::analyzeQuasiQuote(const ASTQuasiQuoteNodePtr &quasiQuote)
{
    auto pattern = quasiQuote->expression;
    transformChildNode(pattern);
    auto patternType = pattern->getType();

    auto patternLiteralValue = basicMakeObject<ASTLiteralValueNode> ();
    patternLiteralValue->value = pattern;
    patternLiteralValue->type = patternType;
    patternLiteralValue->analyzedType = patternType;
    
    auto result = basicMakeObject<ASTQuasiQuotePatternExpansionNode> ();
    result->sourcePosition = quasiQuote->sourcePosition;
    result->pattern = patternLiteralValue;
    result->arguments = patternExpansionArguments;
    result->analyzedType = patternType;
    return result;
}

AnyValuePtr ASTQuasiQuoteAnalyzer::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node)
{
    auto argumentNode = basicMakeObject<ASTQuasiQuotePatternExpansionArgumentNode> ();
    argumentNode->argumentIndex = patternExpansionArguments.size();

    auto expansionArgument = analyzer->analyzeNodeIfNeededWithExpectedType(node->expression, ASTNode::__staticType__());
    patternExpansionArguments.push_back(expansionArgument);
    argumentNode->analyzedType = ASTNode::__staticType__();

    return argumentNode;
}


} // End of namespace Environment
} // End of namespace Sysmel