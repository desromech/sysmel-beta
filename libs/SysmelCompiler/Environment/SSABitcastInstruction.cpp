#include "Environment/SSABitcastInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSABitcastInstruction> SSABitcastInstructionTypeRegistration;

bool SSABitcastInstruction::isSSABitcastInstruction() const
{
    return true;
}

AnyValuePtr SSABitcastInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitBitcastInstruction(selfFromThis());
}

std::string SSABitcastInstruction::getMnemonic() const
{
    return "upcast";
}

} // End of namespace Environment
} // End of namespace Sysmel