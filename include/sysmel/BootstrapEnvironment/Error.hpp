#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_ERROR_HPP
#pragma once

#include "Exception.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Error);

/**
 * Basic error
 */
class Error : public SubtypeOf<Exception, Error>
{
public:
    static constexpr char const __typeName__[] = "Error";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_ERROR_HPP
