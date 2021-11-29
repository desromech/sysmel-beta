#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAFunction> SSAFunctionTypeRegistration;

bool SSAFunction::isSSAFunction() const
{
    return true;
}

AnyValuePtr SSAFunction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitFunction(selfFromThis());
}

SExpression SSAFunction::asFullSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"function"}},
        name ? name->asSExpression() : nullptr,
        functionalType ? functionalType->asSExpression() : nullptr,
    }};
}

AnyValuePtr SSAFunction::getName() const
{
    return name;
}

TypePtr SSAFunction::getValueType() const
{
    return functionalType;
}

const FunctionalTypePtr &SSAFunction::getFunctionalType()
{
    return functionalType;
}

void SSAFunction::initializeWithNameAndType(const AnyValuePtr &newName, const FunctionalTypePtr &newFunctionalType)
{
    name = newName;
    functionalType = newFunctionalType;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius