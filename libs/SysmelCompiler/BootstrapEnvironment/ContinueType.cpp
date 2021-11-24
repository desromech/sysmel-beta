#include "sysmel/BootstrapEnvironment/ContinueType.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ContinueType> returnTypeRegistration;

TypePtr Type::getContinueType()
{
    return ContinueType::__staticType__();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius