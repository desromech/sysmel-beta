#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_DO_WHILE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_DO_WHILE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTDoWhileNode : public SubtypeOf<ASTNode, ASTDoWhileNode>
{
public:
    static constexpr char const __typeName__[] = "ASTDoWhileNode";

    virtual bool isASTDoWhileNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr bodyExpression;
    ASTNodePtr condition;
    ASTNodePtr continueExpression;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_DO_WHILE_NODE_HPP