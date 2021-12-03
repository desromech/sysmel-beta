#include "sysmel/BootstrapEnvironment/LocalVariable.hpp"
#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LocalVariable> localVariableTypeRegistration;

bool LocalVariable::isLocalVariable() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius