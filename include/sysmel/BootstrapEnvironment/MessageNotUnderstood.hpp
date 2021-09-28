#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_MESSAGE_NOT_UNDERSTOOD_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_MESSAGE_NOT_UNDERSTOOD_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class MessageNotUnderstood : public SubtypeOf<Error, MessageNotUnderstood>
{
public:
    static constexpr char const __typeName__[] = "MessageNotUnderstood";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_MESSAGE_NOT_UNDERSTOOD_HPP
