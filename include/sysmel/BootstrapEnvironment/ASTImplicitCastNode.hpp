#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP
#pragma once

#include "ASTCastNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for accessing a variable.
 */
class ASTImplicitCastNode : public SubtypeOf<ASTCastNode, ASTImplicitCastNode>
{
public:
    static constexpr char const __typeName__[] = "ASTImplicitCastNode";

    virtual bool isASTImplicitCastNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP