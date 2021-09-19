#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LITERAL_VALUE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LITERAL_VALUE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am an AST node that wraps a specific literal value with a type.
 */
class ASTLiteralValueNode : public SubtypeOf<ASTNode, ASTLiteralValueNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLiteralValueNode";

    virtual bool isASTLiteralValueNode() const override;
    virtual SExpression asSExpression() const override;

    void setValueAndType(const AnyValuePtr &theValue);

    AnyValuePtr value;
    TypePtr type;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LITERAL_VALUE_NODE_HPP