#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_DOMAIN_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_DOMAIN_ERROR_HPP
#pragma once

#include "ArithmeticError.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class DomainError : public SubtypeOf<ArithmeticError, DomainError>
{
public:
    static constexpr char const __typeName__[] = "DomainError";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_DOMAIN_ERROR_HPP
