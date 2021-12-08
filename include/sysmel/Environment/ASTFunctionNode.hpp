#ifndef SYSMEL_ENVIRONMENT_AST_FUNCTION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_FUNCTION_NODE_HPP
#pragma once

#include "ASTFunctionalNode.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_FUNCTION_NODE_HPP