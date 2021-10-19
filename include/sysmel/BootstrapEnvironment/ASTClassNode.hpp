#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CLASS_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CLASS_NODE_HPP
#pragma once

#include "ASTTypeNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTClassNode : public SubtypeOf<ASTTypeNode, ASTClassNode>
{
public:
    static constexpr char const __typeName__[] = "ASTClassNode";

    virtual bool isASTClassNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr superclass;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CLASS_NODE_HPP