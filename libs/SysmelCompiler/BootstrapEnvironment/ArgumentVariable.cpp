#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ArgumentVariable> localVariableTypeRegistration;

TypePtr Type::getArgumentVariableType()
{
    return ArgumentVariable::__staticType__();
}

bool ArgumentVariable::isArgumentVariable() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius