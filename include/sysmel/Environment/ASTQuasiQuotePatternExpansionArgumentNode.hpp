#ifndef SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_PATTERN_EXPANSION_ARGUMENT_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_PATTERN_EXPANSION_ARGUMENT_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTQuasiQuotePatternExpansionArgumentNode : public SubtypeOf<ASTNode, ASTQuasiQuotePatternExpansionArgumentNode>
{
public:
    static constexpr char const __typeName__[] = "ASTQuasiQuotePatternExpansionArgumentNode";

    virtual bool isASTQuasiQuotePatternExpansionArgumentNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    bool isSplice = false;
    uint32_t argumentIndex = 0; 
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_QUASI_QUOTE_PATTERN_EXPANSION_NODE_HPP