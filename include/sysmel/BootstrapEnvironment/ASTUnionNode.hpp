#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_UNION_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_UNION_NODE_HPP
#pragma once

#include "ASTTypeNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTUnionNode : public SubtypeOf<ASTTypeNode, ASTUnionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTUnionNode";

    virtual bool isASTUnionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_UNION_NODE_HPP