#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FIELD_VARIABLE_ACCESS_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FIELD_VARIABLE_ACCESS_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable);

/**
 * I am a node that is used for accessing a variable.
 */
class ASTFieldVariableAccessNode : public SubtypeOf<ASTNode, ASTFieldVariableAccessNode>
{
public:
    static constexpr char const __typeName__[] = "ASTFieldVariableAccessNode";

    virtual bool isASTFieldVariableAccessNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr aggregate;
    FieldVariablePtr fieldVariable;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FIELD_VARIABLE_ACCESS_NODE_HPP