#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NAMESPACE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NAMESPACE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"
#include "MethodDispatchMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a functional object.
 */
class ASTNamespaceNode : public SubtypeOf<ASTProgramEntityNode, ASTNamespaceNode>
{
public:
    static constexpr char const __typeName__[] = "ASTNamespaceNode";

    virtual bool isASTNamespaceNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr name;
    ASTNodePtr body;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NAMESPACE_NODE_HPP