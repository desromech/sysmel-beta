#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BREAK_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BREAK_TYPE_HPP
#pragma once

#include "ControlFlowEscapeType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BreakType);

/**
 * I am literal number value.
 */
class BreakType : public SubtypeOf<ControlFlowEscapeType, BreakType>
{
public:
    static constexpr char const __typeName__[] = "BreakType";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BREAK_TYPE_HPP