#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class CannotEvaluateMessage : public SubtypeOf<Error, CannotEvaluateMessage>
{
public:
    static constexpr char const __typeName__[] = "CannotEvaluateMessage";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_HPP
