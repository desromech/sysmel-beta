#ifndef SYSMEL_ENVIRONMENT_TRAP_REASON_HPP
#define SYSMEL_ENVIRONMENT_TRAP_REASON_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Trap reason code.
 */
enum class TrapReason
{
    Generic = 0,
    PatternMatchingFailure,
    OutOfBounds,
};

SYSMEL_COMPILER_LIB_EXPORT const char *trapReasonToString(TrapReason reason);

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_TRAP_NODE_HPP
