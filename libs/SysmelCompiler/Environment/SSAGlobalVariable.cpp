#include "Environment/SSAGlobalVariable.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel