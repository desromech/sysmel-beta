#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_ARGUMENT_COUNT_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_ARGUMENT_COUNT_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class ArgumentCountError : public SubtypeOf<Error, ArgumentCountError>
{
public:
    static constexpr char const __typeName__[] = "ArgumentCountError";

    size_t expectedCount;
    size_t callCount;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_ARGUMENT_COUNT_ERROR_HPP
