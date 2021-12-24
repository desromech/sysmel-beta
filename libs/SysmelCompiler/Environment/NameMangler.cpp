#include "Environment/NameMangler.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<NameMangler> NameManglerTypeRegistration;

std::string NameMangler::mangleType(const TypePtr &)
{
    SysmelSelfSubclassResponsibility();
}

std::string NameMangler::mangleTypeInfo(const TypePtr &)
{
    SysmelSelfSubclassResponsibility();
}

std::string NameMangler::mangleProgramEntity(const ProgramEntityPtr &)
{
    SysmelSelfSubclassResponsibility();
}

std::string NameMangler::mangleSSAProgramEntity(const SSAProgramEntityPtr &)
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace Environment
} // End of namespace Sysmel