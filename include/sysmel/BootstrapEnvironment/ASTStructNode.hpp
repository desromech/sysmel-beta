#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_STRUCT_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_STRUCT_NODE_HPP
#pragma once

#include "ASTTypeNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTStructNode : public SubtypeOf<ASTTypeNode, ASTStructNode>
{
public:
    static constexpr char const __typeName__[] = "ASTStructNode";

    virtual bool isASTStructNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_STRUCT_NODE_HPP