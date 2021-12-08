#include "Environment/ContinueType.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ContinueType> returnTypeRegistration;

TypePtr Type::getContinueType()
{
    return ContinueType::__staticType__();
}

} // End of namespace Environment
} // End of namespace Sysmel