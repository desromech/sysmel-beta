#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_QUOTE_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_QUOTE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTQuoteNode : public SubtypeOf<ASTNode, ASTQuoteNode>
{
public:
    static constexpr char const __typeName__[] = "ASTQuoteNode";

    virtual bool isASTQuoteNode() const override;
    virtual AnyValuePtr encodeAsSExpression() const override;

    ASTNodePtr expression;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_QUOTE_NODE_HPP