#include "sysmel/BootstrapEnvironment/Auto.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<Auto> voidRegistration;

TypePtr Type::getAutoType()
{
    return Auto::__staticType__();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius