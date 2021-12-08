#include "Environment/BreakType.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<BreakType> returnTypeRegistration;

TypePtr Type::getBreakType()
{
    return BreakType::__staticType__();
}

} // End of namespace Environment
} // End of namespace Sysmel