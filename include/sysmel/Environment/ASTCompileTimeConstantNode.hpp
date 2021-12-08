#ifndef SYSMEL_ENVIRONMENT_AST_COMPILE_TIME_CONSTANT_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_COMPILE_TIME_CONSTANT_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a variable.
 */
class ASTCompileTimeConstantNode : public SubtypeOf<ASTProgramEntityNode, ASTCompileTimeConstantNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCompileTimeConstantNode";

    virtual bool isASTCompileTimeConstantNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;

    ASTNodePtr name;
    ASTNodePtr type;
    ASTNodePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_COMPILE_TIME_CONSTANT_NODE_HPP