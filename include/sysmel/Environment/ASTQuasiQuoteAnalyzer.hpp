#ifndef SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_ANALYZER_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_ANALYZER_VISITOR_HPP
#pragma once

#include "ASTSemanticAnalyzer.hpp"
#include "ASTTransformVisitor.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ASTSourcePosition);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTQuasiQuoteAnalyzer : public SubtypeOf<ASTTransformVisitor, ASTQuasiQuoteAnalyzer>
{
public:
    static constexpr char const __typeName__[] = "ASTQuasiQuoteAnalyzer";

    ASTQuasiQuotePatternExpansionNodePtr analyzeQuasiQuote(const ASTQuasiQuoteNodePtr &quasiQuote);
    virtual AnyValuePtr visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node) override;

    ASTSemanticAnalyzerPtr analyzer;
    ASTNodePtrList patternExpansionArguments;
    ASTSourcePositionPtrList patternExpansionArgumentSources;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_ANALYZER_VISITOR_HPP