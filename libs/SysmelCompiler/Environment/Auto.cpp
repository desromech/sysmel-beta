#include "Environment/Auto.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<Auto> voidRegistration;

TypePtr Type::getAutoType()
{
    return Auto::__staticType__();
}

} // End of namespace Environment
} // End of namespace Sysmel