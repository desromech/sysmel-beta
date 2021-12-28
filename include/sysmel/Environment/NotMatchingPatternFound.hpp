#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_MATCHING_PATTERN_FOUND_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_MATCHING_PATTERN_FOUND_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT NotMatchingPatternFound : public SubtypeOf<Error, NotMatchingPatternFound>
{
public:
    static constexpr char const __typeName__[] = "NotMatchingPatternFound";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_MATCHING_PATTERN_FOUND_HPP
