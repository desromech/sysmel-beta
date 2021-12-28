#ifndef SYSMEL_ENVIRONMENT_AST_LEXICAL_SCOPE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_LEXICAL_SCOPE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope);

/**
 * I an AST node that introduces a new lexical scope used for identifier reference lookup.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTLexicalScopeNode : public SubtypeOf<ASTNode, ASTLexicalScopeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLexicalScopeNode";

    virtual bool isASTLexicalScopeNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual ASTNodePtr asInlinedBlockBodyNode() override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr body;
    LexicalScopePtr analyzedScope;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_LEXICAL_SCOPE_NODE_HPP