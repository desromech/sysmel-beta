#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_OVERLOAD_PATTERN_MATCHING_METHOD_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_OVERLOAD_PATTERN_MATCHING_METHOD_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT CannotOverloadPatternMatchingMethod : public SubtypeOf<Error, CannotOverloadPatternMatchingMethod>
{
public:
    static constexpr char const __typeName__[] = "CannotOverloadPatternMatchingMethod";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_OVERLOAD_PATTERN_MATCHING_METHOD_HPP
