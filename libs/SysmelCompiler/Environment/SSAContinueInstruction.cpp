#include "Environment/SSAContinueInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel