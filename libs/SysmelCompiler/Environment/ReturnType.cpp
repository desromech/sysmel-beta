#include "Environment/ReturnType.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ReturnType> returnTypeRegistration;

TypePtr Type::getReturnType()
{
    return ReturnType::__staticType__();
}

} // End of namespace Environment
} // End of namespace Sysmel