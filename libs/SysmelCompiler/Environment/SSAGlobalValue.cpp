#include "Environment/SSAGlobalValue.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel