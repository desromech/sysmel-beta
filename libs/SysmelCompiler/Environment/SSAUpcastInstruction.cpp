#include "Environment/SSAUpcastInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAUpcastInstruction> SSAUpcastInstructionTypeRegistration;

bool SSAUpcastInstruction::isSSAUpcastInstruction() const
{
    return true;
}

AnyValuePtr SSAUpcastInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitUpcastInstruction(selfFromThis());
}

std::string SSAUpcastInstruction::getMnemonic() const
{
    return "upcast";
}

} // End of namespace Environment
} // End of namespace Sysmel