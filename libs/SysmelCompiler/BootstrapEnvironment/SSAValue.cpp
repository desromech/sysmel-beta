#include "sysmel/BootstrapEnvironment/SSAValue.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAValue> SSAValueTypeRegistration;

bool SSAValue::isSSAValue() const
{
    return true;
}

bool SSAValue::isTerminatorInstruction() const
{
    return false;
}

AnyValuePtr SSAValue::accept(const SSAValueVisitorPtr &)
{
    SysmelSelfSubclassResponsibility();
}

SExpression SSAValue::asFullSExpression() const
{
    return asSExpression();
}

std::string SSAValue::printString() const
{
    return sexpressionToPrettyString(asSExpression());
}

std::string SSAValue::fullPrintString() const
{
    return sexpressionToPrettyString(asFullSExpression());
}

TypePtr SSAValue::getValueType() const
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius