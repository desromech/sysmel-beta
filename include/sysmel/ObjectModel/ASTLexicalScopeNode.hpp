#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_LEXICAL_SCOPE_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_LEXICAL_SCOPE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I an AST node that introduces a new lexical scope used for identifier reference lookup.
 */
class ASTLexicalScopeNode : public SubtypeOf<ASTNode, ASTLexicalScopeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLexicalScopeNode";

    virtual bool isASTLexicalScopeNode() const override;

    AnyValuePtr body;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_LEXICAL_SCOPE_NODE_HPP