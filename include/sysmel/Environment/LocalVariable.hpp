#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LOCAL_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LOCAL_VARIABLE_HPP
#pragma once

#include "FunctionVariable.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a variable program entity.
 */
class LocalVariable : public SubtypeOf<FunctionVariable, LocalVariable>
{
public:
    static constexpr char const __typeName__[] = "LocalVariable";

    virtual bool isLocalVariable() const override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LOCAL_VARIABLE_HPP