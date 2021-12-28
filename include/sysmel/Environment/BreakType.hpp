#ifndef SYSMEL_ENVIRONMENT_BREAK_TYPE_HPP
#define SYSMEL_ENVIRONMENT_BREAK_TYPE_HPP
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
class SYSMEL_COMPILER_LIB_EXPORT BreakType : public SubtypeOf<ControlFlowEscapeType, BreakType>
{
public:
    static constexpr char const __typeName__[] = "BreakType";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_BREAK_TYPE_HPP