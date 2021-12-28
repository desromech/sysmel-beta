#ifndef SYSMEL_ENVIRONMENT_AST_LOCAL_VARIABLE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_LOCAL_VARIABLE_NODE_HPP
#pragma once

#include "ASTVariableNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTLocalVariableNode : public SubtypeOf<ASTVariableNode, ASTLocalVariableNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLocalVariableNode";

    virtual bool isASTLocalVariableNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_LOCAL_VARIABLE_NODE_HPP