#include "sysmel/BootstrapEnvironment/SSAReturnFromFunctionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAReturnFromFunctionInstruction> SSAReturnFromFunctionInstructionTypeRegistration;

bool SSAReturnFromFunctionInstruction::isSSAReturnFromFunctionInstruction() const
{
    return true;
}

AnyValuePtr SSAReturnFromFunctionInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitReturnFromFunctionInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius