#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AUTO_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AUTO_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Auto);

/**
 * I am literal number value.
 */
class Auto : public SubtypeOf<AnyValue, Auto>
{
public:
    static constexpr char const __typeName__[] = "Auto";
    static constexpr bool __isDynamicCompileTimeType__ = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AUTO_HPP