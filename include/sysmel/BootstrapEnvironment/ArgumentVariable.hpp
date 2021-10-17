#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARGUMENT_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARGUMENT_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);

/**
 * I am a variable program entity.
 */
class ArgumentVariable : public SubtypeOf<Variable, ArgumentVariable>
{
public:
    static constexpr char const __typeName__[] = "ArgumentVariable";

    virtual bool isArgumentVariable() const override;

    void setType(const TypePtr &type);
    void setDeclarationNode(const ASTArgumentDefinitionNodePtr &node);
    void setDefinitionNode(const ASTArgumentDefinitionNodePtr &node);
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARGUMENT_VARIABLE_HPP