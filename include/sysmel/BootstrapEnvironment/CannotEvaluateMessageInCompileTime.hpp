#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_IN_COMPILE_TIME_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_IN_COMPILE_TIME_HPP
#pragma once

#include "CannotEvaluateMessage.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class CannotEvaluateMessageInCompileTime : public SubtypeOf<CannotEvaluateMessage, CannotEvaluateMessageInCompileTime>
{
public:
    static constexpr char const __typeName__[] = "CannotEvaluateMessageInCompileTime";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_IN_COMPILE_TIME_HPP
