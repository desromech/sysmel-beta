#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BREAK_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BREAK_TYPE_HPP
#pragma once

#include "ControlFlowEscapeType.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BREAK_TYPE_HPP