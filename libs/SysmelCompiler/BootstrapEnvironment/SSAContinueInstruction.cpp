#include "sysmel/BootstrapEnvironment/SSAContinueInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAContinueInstruction> SSAContinueInstructionTypeRegistration;

bool SSAContinueInstruction::isSSAContinueInstruction() const
{
    return true;
}

bool SSAContinueInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAContinueInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitContinueInstruction(selfFromThis());
}

TypePtr SSAContinueInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSAContinueInstruction::getMnemonic() const
{
    return "continue";
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius