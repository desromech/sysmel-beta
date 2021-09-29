#include "sysmel/BootstrapEnvironment/Pragma.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Pragma> pragmaTypeRegistration;

TypePtr Type::getPragmaType()
{
    return Pragma::__staticType__();
}

bool Pragma::isPragma() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius