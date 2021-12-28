#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_MESSAGE_NOT_UNDERSTOOD_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_MESSAGE_NOT_UNDERSTOOD_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT MessageNotUnderstood : public SubtypeOf<Error, MessageNotUnderstood>
{
public:
    static constexpr char const __typeName__[] = "MessageNotUnderstood";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_MESSAGE_NOT_UNDERSTOOD_HPP
