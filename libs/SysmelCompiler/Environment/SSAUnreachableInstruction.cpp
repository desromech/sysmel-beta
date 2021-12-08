#include "Environment/SSAUnreachableInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAUnreachableInstruction> SSAUnreachableInstructionTypeRegistration;

bool SSAUnreachableInstruction::isSSAUnreachableInstruction() const
{
    return true;
}

bool SSAUnreachableInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAUnreachableInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitUnreachableInstruction(selfFromThis());
}

std::string SSAUnreachableInstruction::getMnemonic() const
{
    return "unreachable";
}

TypePtr SSAUnreachableInstruction::getValueType() const
{
    return Type::getVoidType();
}
} // End of namespace Environment
} // End of namespace Sysmel