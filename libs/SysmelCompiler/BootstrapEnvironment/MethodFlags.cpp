#include "sysmel/BootstrapEnvironment/MethodFlags.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SExpression methodFlagsToSExpression(MethodFlags flags)
{
    SExpressionList result;

#define AddFlag(flagName) \
    if((flags & MethodFlags::flagName) != MethodFlags::None) \
        result.elements.push_back(SExpressionIdentifier{{#flagName}});

    // Kind
    AddFlag(Macro);
    AddFlag(Fallback);
    AddFlag(MessageMethod);

    // Side effects
    AddFlag(Const);
    AddFlag(Pure);

    // Unwinding semantics
    AddFlag(NoThrow);
    AddFlag(ReturnsTwice);

    // Type conversions
    AddFlag(Constructor);
    AddFlag(Conversion);
    AddFlag(Explicit);

    // Dispatch mode
    AddFlag(Abstract);
    AddFlag(Final);
    AddFlag(Override);
    AddFlag(Virtual);
    AddFlag(Static);

    if(result.elements.empty())
        return SExpressionIdentifier{{"None"}};
    else if(result.elements.size() == 0)
        return result.elements[0];
    else
        return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius