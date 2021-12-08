#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTTypeNode : public SubtypeOf<ASTProgramEntityNode, ASTTypeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTypeNode";

    virtual bool isASTTypeNode() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr name;
    ASTNodePtr body;
    ASTNodePtr alignment;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP