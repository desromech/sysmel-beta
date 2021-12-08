#include "Environment/SSAModule.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAModule> SSAModuleTypeRegistration;

bool SSAModule::isSSAModule() const
{
    return true;
}

AnyValuePtr SSAModule::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitModule(selfFromThis());
}

SExpression SSAModule::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"module"}},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression SSAModule::asFullDefinitionSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"module"}},
        validAnyValue(name)->asSExpression(),
        globalNamespace->asFullDefinitionSExpression(),
    }};
}

AnyValuePtr SSAModule::getName() const
{
    return name;
}

void SSAModule::setName(const AnyValuePtr &newName)
{
    name = newName;
}

const SSAValuePtr &SSAModule::getGlobalNamespace() const
{
    return globalNamespace;
}

void SSAModule::setGlobalNamespace(const SSAValuePtr &newGlobalNamespace)
{
    globalNamespace = newGlobalNamespace;
}

} // End of namespace Environment
} // End of namespace Sysmel