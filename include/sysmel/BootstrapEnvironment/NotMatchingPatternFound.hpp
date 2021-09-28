#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_MATCHING_PATTERN_FOUND_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_MATCHING_PATTERN_FOUND_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class NotMatchingPatternFound : public SubtypeOf<Error, NotMatchingPatternFound>
{
public:
    static constexpr char const __typeName__[] = "NotMatchingPatternFound";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_MATCHING_PATTERN_FOUND_HPP
