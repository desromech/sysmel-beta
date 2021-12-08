#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_UNSUPPORTED_OPERATION_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_UNSUPPORTED_OPERATION_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class UnsupportedOperation : public SubtypeOf<Error, UnsupportedOperation>
{
public:
    static constexpr char const __typeName__[] = "UnsupportedOperation";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_UNSUPPORTED_OPERATION_HPP
