#include "Environment/NotInCompileTimeError.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<NotInCompileTimeError> NotInCompileTimeErrorTypeRegistration;

} // End of namespace Environment
} // End of namespace Sysmel