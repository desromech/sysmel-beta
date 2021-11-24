#include "sysmel/BootstrapEnvironment/BreakType.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<BreakType> returnTypeRegistration;

TypePtr Type::getBreakType()
{
    return BreakType::__staticType__();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius