#include "sysmel/BootstrapEnvironment/SSAGlobalVariable.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAGlobalVariable> SSAGlobalVariableTypeRegistration;

bool SSAGlobalVariable::isSSAGlobalVariable() const
{
    return true;
}

AnyValuePtr SSAGlobalVariable::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitGlobalVariable(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius