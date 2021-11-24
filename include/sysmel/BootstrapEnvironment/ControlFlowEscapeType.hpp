#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONTROL_FLOW_ESCAPE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONTROL_FLOW_ESCAPE_TYPE_HPP
#pragma once

#include "Void.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ControlFlowEscapeType);

/**
 * I am used for marking a control flow escape result type. (return, break, continue)
 */
class ControlFlowEscapeType : public SubtypeOf<Void, ControlFlowEscapeType>
{
public:
    static constexpr char const __typeName__[] = "ControlFlowEscapeType";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONTROL_FLOW_ESCAPE_TYPE_HPP