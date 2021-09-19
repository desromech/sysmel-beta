#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_QUASI_UNQUOTE_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_QUASI_UNQUOTE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTQuasiUnquoteNode : public SubtypeOf<ASTNode, ASTQuasiUnquoteNode>
{
public:
    static constexpr char const __typeName__[] = "ASTQuasiUnquoteNode";

    virtual bool isASTQuasiUnquoteNode() const override;
    virtual AnyValuePtr encodeAsSExpression() const override;

    ASTNodePtr expression;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_QUASI_UNQUOTE_NODE_HPP