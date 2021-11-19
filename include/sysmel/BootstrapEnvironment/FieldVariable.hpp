#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FIELD_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FIELD_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable);

/**
 * I am a variable program entity.
 */
class FieldVariable : public SubtypeOf<Variable, FieldVariable>
{
public:
    static constexpr char const __typeName__[] = "FieldVariable";

    virtual bool isFieldVariable() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FIELD_VARIABLE_HPP