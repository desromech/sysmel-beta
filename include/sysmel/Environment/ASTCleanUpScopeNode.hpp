#ifndef SYSMEL_ENVIRONMENT_AST_CLEANUP_SCOPE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_CLEANUP_SCOPE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CleanUpScope);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTCleanUpScopeNode : public SubtypeOf<ASTNode, ASTCleanUpScopeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCleanUpScopeNode";

    virtual bool isASTCleanUpScopeNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual ASTNodePtr asInlinedBlockBodyNode() override;
    
    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    virtual ASTPragmaNodePtr getPragmaNamed(const AnyValuePtr &requestedPragmaSelector) override;

    ASTNodePtr body;

    CleanUpScopePtr analyzedScope;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_CLEANUP_SCOPE_NODE_HPP