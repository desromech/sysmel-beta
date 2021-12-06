#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARGUMENT_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARGUMENT_VARIABLE_HPP
#pragma once

#include "FunctionVariable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);

/**
 * I am a variable program entity.
 */
class ArgumentVariable : public SubtypeOf<FunctionVariable, ArgumentVariable>
{
public:
    static constexpr char const __typeName__[] = "ArgumentVariable";

    virtual bool isArgumentVariable() const override;

    void setType(const TypePtr &type);
    void setName(const AnyValuePtr &newName);
    void setArgumentDeclarationNode(const ASTArgumentDefinitionNodePtr &node);
    void setArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node);

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;
    
    bool isImplicit = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARGUMENT_VARIABLE_HPP