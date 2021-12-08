#include "Environment/SSADowncastInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSADowncastInstruction> SSADowncastInstructionTypeRegistration;

bool SSADowncastInstruction::isSSADowncastInstruction() const
{
    return true;
}

AnyValuePtr SSADowncastInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDowncastInstruction(selfFromThis());
}

std::string SSADowncastInstruction::getMnemonic() const
{
    return "upcast";
}

} // End of namespace Environment
} // End of namespace Sysmel