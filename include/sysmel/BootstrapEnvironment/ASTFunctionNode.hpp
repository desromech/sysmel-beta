#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FUNCTION_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FUNCTION_NODE_HPP
#pragma once

#include "ASTFunctionalNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for defining functions.
 */
class ASTFunctionNode : public SubtypeOf<ASTFunctionalNode, ASTFunctionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTFunctionNode";

    virtual bool isASTFunctionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FUNCTION_NODE_HPP