#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VALUE_BOX_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VALUE_BOX_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a box that is used for holding a value. I am typically used for storing mutable variable values.
 */
class ValueBox : public SubtypeOf<CompilerObject, ValueBox>
{
public:
    static constexpr char const __typeName__[] = "ValueBox";

    virtual bool isValueBox() const override;

    AnyValuePtr value;
    TypePtr type;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VALUE_BOX_HPP
