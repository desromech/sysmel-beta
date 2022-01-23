#include "Environment/TrapReason.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_COMPILER_LIB_EXPORT const char *trapReasonToString(TrapReason reason)
{
    switch(reason)
    {
    default:
    case TrapReason::Generic: return "generic";
    case TrapReason::PatternMatchingFailure: return "patternMatchingFailure";
    case TrapReason::OutOfBounds: return "outOfBounds";
    }
}

} // End of namespace Environment
} // End of namespace Sysmel