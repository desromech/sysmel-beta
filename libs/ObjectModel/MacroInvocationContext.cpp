#include "sysmel/ObjectModel/MacroInvocationContext.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<MacroInvocationContext> macroInvocationContextRegistration;

bool MacroInvocationContext::isMacroInvocationContext() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius