#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_ARGUMENT_COUNT_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_ARGUMENT_COUNT_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT ArgumentCountError : public SubtypeOf<Error, ArgumentCountError>
{
public:
    static constexpr char const __typeName__[] = "ArgumentCountError";

    size_t expectedCount;
    size_t callCount;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_ARGUMENT_COUNT_ERROR_HPP
