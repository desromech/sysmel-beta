#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LEXICAL_SCOPE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LEXICAL_SCOPE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope);

/**
 * I an AST node that introduces a new lexical scope used for identifier reference lookup.
 */
class ASTLexicalScopeNode : public SubtypeOf<ASTNode, ASTLexicalScopeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLexicalScopeNode";

    virtual bool isASTLexicalScopeNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr body;
    LexicalScopePtr analyzedScope;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LEXICAL_SCOPE_NODE_HPP