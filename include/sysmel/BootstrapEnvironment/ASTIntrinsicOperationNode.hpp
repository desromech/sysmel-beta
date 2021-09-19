#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_INTRINSIC_OPERATION_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_INTRINSIC_OPERATION_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that applies a generic intrinsic operation for processing its parameters.
 */
class ASTIntrinsicOperationNode : public SubtypeOf<ASTNode, ASTIntrinsicOperationNode>
{
public:
    static constexpr char const __typeName__[] = "ASTIntrinsicOperationNode";

    virtual bool isASTIntrinsicOperationNode() const override;

    AnyValuePtr operation;
    ASTNodePtrList parameters;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_INTRINSIC_OPERATION_NODE_HPP