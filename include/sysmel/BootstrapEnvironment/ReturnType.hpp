#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RETURN_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RETURN_TYPE_HPP
#pragma once

#include "ControlFlowEscapeType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReturnType);

/**
 * I am literal number value.
 */
class ReturnType : public SubtypeOf<ControlFlowEscapeType, ReturnType>
{
public:
    static constexpr char const __typeName__[] = "ReturnType";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RETURN_TYPE_HPP