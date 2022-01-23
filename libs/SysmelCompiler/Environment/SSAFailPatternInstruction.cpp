#include "Environment/SSAFailPatternInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAFailPatternInstruction> SSAFailPatternInstructionTypeRegistration;

bool SSAFailPatternInstruction::isSSAFailPatternInstruction() const
{
    return true;
}

bool SSAFailPatternInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAFailPatternInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitFailPatternInstruction(selfFromThis());
}

TypePtr SSAFailPatternInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSAFailPatternInstruction::getMnemonic() const
{
    return "patternFail";
}

} // End of namespace Environment
} // End of namespace Sysmel