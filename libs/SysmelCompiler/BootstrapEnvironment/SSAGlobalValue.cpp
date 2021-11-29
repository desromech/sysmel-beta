#include "sysmel/BootstrapEnvironment/SSAGlobalValue.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAGlobalValue> SSAGlobalValueTypeRegistration;

bool SSAGlobalValue::isSSAGlobalValue() const
{
    return true;
}

AnyValuePtr SSAGlobalValue::getName() const
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius