#include "sysmel/BootstrapEnvironment/ReturnType.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ReturnType> returnTypeRegistration;

TypePtr Type::getReturnType()
{
    return ReturnType::__staticType__();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius