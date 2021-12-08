#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_DIVISION_BY_ZERO_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_DIVISION_BY_ZERO_ERROR_HPP
#pragma once

#include "ArithmeticError.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class DivisionByZeroError : public SubtypeOf<ArithmeticError, DivisionByZeroError>
{
public:
    static constexpr char const __typeName__[] = "DivisionByZeroError";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_DIVISION_BY_ZERO_ERROR_HPP
