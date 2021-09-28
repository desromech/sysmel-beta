#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_ARITHMETIC_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_ARITHMETIC_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class ArithmeticError : public SubtypeOf<Error, ArithmeticError>
{
public:
    static constexpr char const __typeName__[] = "ArithmeticError";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_ARITHMETIC_ERROR_HPP
