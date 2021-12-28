#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_SUBCLASS_RESPONSIBILITY_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_SUBCLASS_RESPONSIBILITY_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT SubclassResponsibility : public SubtypeOf<Error, SubclassResponsibility>
{
public:
    static constexpr char const __typeName__[] = "SubclassResponsibility";
};

#define SysmelSelfSubclassResponsibility() \
    signalNewWithMessage<SubclassResponsibility> (__func__)

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_SUBCLASS_RESPONSIBILITY_HPP
