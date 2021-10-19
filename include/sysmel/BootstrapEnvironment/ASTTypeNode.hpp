#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTTypeNode : public SubtypeOf<ASTProgramEntityNode, ASTTypeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTypeNode";

    virtual bool isASTTypeNode() const override;

    ASTNodePtr name;
    ASTNodePtr body;
    ASTNodePtr alignment;
    ASTNodePtr initialValue;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP