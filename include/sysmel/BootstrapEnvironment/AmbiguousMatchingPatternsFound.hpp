#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_AMBIGUOUS_MATCHING_PATTERNS_FOUND_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_AMBIGUOUS_MATCHING_PATTERNS_FOUND_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class AmbiguousMatchingPatternsFound : public SubtypeOf<Error, AmbiguousMatchingPatternsFound>
{
public:
    static constexpr char const __typeName__[] = "AmbiguousMatchingPatternsFound";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_AMBIGUOUS_MATCHING_PATTERNS_FOUND_HPP
