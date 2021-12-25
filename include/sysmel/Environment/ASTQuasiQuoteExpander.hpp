#ifndef SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_EXPANDER_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_EXPANDER_VISITOR_HPP
#pragma once

#include "ASTSemanticAnalyzer.hpp"
#include "ASTTransformVisitor.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTQuasiQuoteExpander : public SubtypeOf<ASTTransformVisitor, ASTQuasiQuoteExpander>
{
public:
    static constexpr char const __typeName__[] = "ASTQuasiQuoteExpander";

    ASTNodePtr expandPatternWithArguments(const ASTNodePtr &pattern, const AnyValuePtrList &newExpansionArguments);

    virtual AnyValuePtr visitQuasiQuotePatternExpansionArgumentNode(const ASTQuasiQuotePatternExpansionArgumentNodePtr &node) override;

    ASTSemanticAnalyzerPtr analyzer;
    AnyValuePtrList expansionArguments;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_EXPANDER_VISITOR_HPP