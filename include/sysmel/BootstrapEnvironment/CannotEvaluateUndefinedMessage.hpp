#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_UNDEFINED_MESSAGE_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_UNDEFINED_MESSAGE_HPP
#pragma once

#include "CannotEvaluateMessage.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class CannotEvaluateUndefinedMessage : public SubtypeOf<CannotEvaluateMessage, CannotEvaluateUndefinedMessage>
{
public:
    static constexpr char const __typeName__[] = "CannotEvaluateUndefinedMessage";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_UNDEFINED_MESSAGE_HPP