#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FIELD_VARIABLE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FIELD_VARIABLE_NODE_HPP
#pragma once

#include "ASTVariableNode.hpp"
#include "ProgramEntityVisibility.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a program entity.
 */
class ASTFieldVariableNode : public SubtypeOf<ASTVariableNode, ASTFieldVariableNode>
{
public:
    static constexpr char const __typeName__[] = "ASTFieldVariableNode";

    virtual bool isASTFieldVariableNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FIELD_VARIABLE_NODE_HPP