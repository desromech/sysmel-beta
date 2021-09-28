#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_SUBCLASS_RESPONSIBILITY_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_SUBCLASS_RESPONSIBILITY_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Basic error
 */
class SubclassResponsibility : public SubtypeOf<Error, SubclassResponsibility>
{
public:
    static constexpr char const __typeName__[] = "SubclassResponsibility";
};

#define SysmelSelfSubclassResponsibility() \
    signalNewWithMessage<SubclassResponsibility> (__func__)

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_SUBCLASS_RESPONSIBILITY_HPP
