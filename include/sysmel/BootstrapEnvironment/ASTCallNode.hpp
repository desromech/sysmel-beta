#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CALL_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CALL_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that applies a generic intrinsic operation for processing its parameters.
 */
class ASTCallNode : public SubtypeOf<ASTNode, ASTCallNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCallNode";

    virtual bool isASTCallNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr function;
    ASTNodePtrList arguments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CALL_NODE_HPP