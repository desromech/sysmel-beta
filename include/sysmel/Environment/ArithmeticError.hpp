#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_ARITHMETIC_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_ARITHMETIC_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class ArithmeticError : public SubtypeOf<Error, ArithmeticError>
{
public:
    static constexpr char const __typeName__[] = "ArithmeticError";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_ARITHMETIC_ERROR_HPP
