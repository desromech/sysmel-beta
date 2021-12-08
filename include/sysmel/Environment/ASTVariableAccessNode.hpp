#ifndef SYSMEL_ENVIRONMENT_AST_VARIABLE_ACCESS_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_VARIABLE_ACCESS_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Variable);

/**
 * I am a node that is used for accessing a variable.
 */
class ASTVariableAccessNode : public SubtypeOf<ASTNode, ASTVariableAccessNode>
{
public:
    static constexpr char const __typeName__[] = "ASTVariableAccessNode";

    virtual bool isASTVariableAccessNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    VariablePtr variable;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_VARIABLE_ACCESS_NODE_HPP