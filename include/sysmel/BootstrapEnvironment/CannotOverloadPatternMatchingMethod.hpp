#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_OVERLOAD_PATTERN_MATCHING_METHOD_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_OVERLOAD_PATTERN_MATCHING_METHOD_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class CannotOverloadPatternMatchingMethod : public SubtypeOf<Error, CannotOverloadPatternMatchingMethod>
{
public:
    static constexpr char const __typeName__[] = "CannotOverloadPatternMatchingMethod";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_OVERLOAD_PATTERN_MATCHING_METHOD_HPP
