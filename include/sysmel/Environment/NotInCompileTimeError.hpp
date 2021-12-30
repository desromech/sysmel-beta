#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_IN_COMPILE_TIME_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_IN_COMPILE_TIME_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT NotInCompileTimeError : public SubtypeOf<Error, NotInCompileTimeError>
{
public:
    static constexpr char const __typeName__[] = "NotInCompileTimeError";
};

#define SysmelSelfNotInCompileTimeError() \
    signalNewWithMessage<NotInCompileTimeError> (__func__)

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_NOT_IN_COMPILE_TIME_ERROR_HPP
