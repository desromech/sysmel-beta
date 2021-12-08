#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_DOMAIN_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_DOMAIN_ERROR_HPP
#pragma once

#include "ArithmeticError.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class DomainError : public SubtypeOf<ArithmeticError, DomainError>
{
public:
    static constexpr char const __typeName__[] = "DomainError";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_DOMAIN_ERROR_HPP
