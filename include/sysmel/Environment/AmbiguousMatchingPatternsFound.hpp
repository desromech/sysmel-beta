#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_AMBIGUOUS_MATCHING_PATTERNS_FOUND_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_AMBIGUOUS_MATCHING_PATTERNS_FOUND_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class AmbiguousMatchingPatternsFound : public SubtypeOf<Error, AmbiguousMatchingPatternsFound>
{
public:
    static constexpr char const __typeName__[] = "AmbiguousMatchingPatternsFound";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_AMBIGUOUS_MATCHING_PATTERNS_FOUND_HPP
