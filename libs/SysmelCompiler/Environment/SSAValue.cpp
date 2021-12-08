#include "Environment/SSAValue.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

SExpression SSAValue::asFullDefinitionSExpression() const
{
    return asSExpression();
}

std::string SSAValue::printString() const
{
    return sexpressionToPrettyString(asSExpression());
}

std::string SSAValue::fullPrintString() const
{
    return sexpressionToPrettyString(asFullDefinitionSExpression());
}

TypePtr SSAValue::getValueType() const
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace Environment
} // End of namespace Sysmel