#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LOCAL_IMMUTABLE_ACCESS_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LOCAL_IMMUTABLE_ACCESS_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Variable);

/**
 * I am a node that is used for accessing a variable.
 */
class ASTLocalImmutableAccessNode : public SubtypeOf<ASTNode, ASTLocalImmutableAccessNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLocalImmutableAccessNode";

    virtual bool isASTLocalImmutableAccessNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;

    AnyValuePtr bindingName;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LOCAL_IMMUTABLE_ACCESS_NODE_HPP