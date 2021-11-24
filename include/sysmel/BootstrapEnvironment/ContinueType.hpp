#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONTINUE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONTINUE_TYPE_HPP
#pragma once

#include "ControlFlowEscapeType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ContinueType);

/**
 * I am literal number value.
 */
class ContinueType : public SubtypeOf<ControlFlowEscapeType, ContinueType>
{
public:
    static constexpr char const __typeName__[] = "ContinueType";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONTINUE_TYPE_HPP