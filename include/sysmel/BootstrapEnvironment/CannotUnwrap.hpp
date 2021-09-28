#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_UNWRAP_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_UNWRAP_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class CannotUnwrap : public SubtypeOf<Error, CannotUnwrap>
{
public:
    static constexpr char const __typeName__[] = "CannotUnwrap";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_UNWRAP_ERROR_HPP
